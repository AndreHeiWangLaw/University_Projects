import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'custom_app_bar.dart';

class ControlPanelPage extends StatefulWidget {
  const ControlPanelPage({Key? key}) : super(key: key);

  @override
  _ControlPanelPageState createState() => _ControlPanelPageState();
}

class _ControlPanelPageState extends State<ControlPanelPage> {
  final DatabaseReference databaseReference =
      FirebaseDatabase.instance.reference();
  Map<String, TextEditingController> humidityControllers = {};
  Map<String, TextEditingController> phMinControllers = {};
  Map<String, TextEditingController> phMaxControllers = {};
  Map<String, TextEditingController> solutionAControllers = {};
  Map<String, TextEditingController> solutionBControllers = {};
  Map<String, TextEditingController> systemVolumeControllers = {};
  Map<String, TextEditingController> lightOnControllers = {};
  Map<String, TextEditingController> lightOffControllers = {};
  Map<String, bool> automationHumidityMap = {};
  Map<String, bool> startInitialDosingMap = {};
  Map<String, bool> lightAutoMap = {};
  Map<String, bool> phAutoMap = {};

  @override
  void dispose() {
    // Dispose all controllers
    humidityControllers.values.forEach((c) => c.dispose());
    phMinControllers.values.forEach((c) => c.dispose());
    phMaxControllers.values.forEach((c) => c.dispose());
    solutionAControllers.values.forEach((c) => c.dispose());
    solutionBControllers.values.forEach((c) => c.dispose());
    systemVolumeControllers.values.forEach((c) => c.dispose());
    lightOnControllers.values.forEach((c) => c.dispose());
    lightOffControllers.values.forEach((c) => c.dispose());
    super.dispose();
  }

  Future<void> _saveHumidityLevel(String systemName) async {
    final String humidityLevel = humidityControllers[systemName]?.text ?? '50';
    await databaseReference
        .child(
            'Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$systemName/HumidityControl')
        .update({
      'humidityLevel': humidityLevel,
    });
  }

  Future<void> _savePHBalance(
      String systemName, String key, String value) async {
    await databaseReference
        .child(
            'Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$systemName/phBalance')
        .update({key: value});
  }

  Future<void> _saveValueToFirebase(
      String systemName, String path, String value) async {
    await databaseReference
        .child(
            'Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$systemName/$path')
        .set(value);
  }

  Future<void> _saveLightControl(
      String systemName, String startTime, String endTime) async {
    await databaseReference
        .child(
            'Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$systemName/LightControl')
        .update({
      'lightON': startTime,
      'lightOFF': endTime,
    });
  }

  Future<void> _saveBooleanValueToFirebase(
      String systemName, String path, bool value) async {
    await databaseReference
        .child(
            'Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$systemName/$path')
        .set(value);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: CustomAppBar(title: 'Control Panel'),
      body: StreamBuilder(
        stream: databaseReference
            .child('Registered Users/${FirebaseAuth.instance.currentUser!.uid}')
            .onValue,
        builder: (context, snapshot) {
          if (snapshot.connectionState == ConnectionState.waiting) {
            return const CircularProgressIndicator();
          } else if (snapshot.hasError) {
            return Text('Error: ${snapshot.error}');
          } else if (!snapshot.hasData ||
              snapshot.data!.snapshot.value == null) {
            return const Text('No data found for this user');
          } else {
            final dataSnapshot = snapshot.data!.snapshot;
            if (dataSnapshot.value is Map) {
              final systemsData = dataSnapshot.value as Map<dynamic, dynamic>;
              List<Widget> systemWidgets =
                  systemsData.entries.map<Widget>((entry) {
                final String systemName = entry.key;
                if (entry.value is Map) {
                  final systemData = entry.value as Map<dynamic, dynamic>;
                  final Map<dynamic, dynamic>? initialDose =
                      systemData['InitialDose'] as Map<dynamic, dynamic>?;

                  humidityControllers[systemName] ??= TextEditingController(
                      text: systemData['HumidityControl']?['humidityLevel']
                              ?.toString() ??
                          '');
                  phMinControllers[systemName] ??= TextEditingController(
                      text:
                          systemData['phBalance']?['phMin']?.toString() ?? '');
                  phMaxControllers[systemName] ??= TextEditingController(
                      text:
                          systemData['phBalance']?['phMax']?.toString() ?? '');
                  solutionAControllers[systemName] ??= TextEditingController(
                      text: initialDose?['Amounts']?['A']?.toString() ?? '');
                  solutionBControllers[systemName] ??= TextEditingController(
                      text: initialDose?['Amounts']?['B']?.toString() ?? '');
                  systemVolumeControllers[systemName] ??= TextEditingController(
                      text: initialDose?['systemVolume']?.toString() ?? '');

                  return Column(
                    children: [
                      ListTile(
                        leading: const Icon(Icons.grain),
                        title: const Text('Humidity Control'),
                        subtitle: Row(
                          children: [
                            IconButton(
                              icon: const Icon(Icons.remove),
                              onPressed: () {
                                int currentValue = int.tryParse(
                                        humidityControllers[systemName]!
                                            .text) ??
                                    50;
                                humidityControllers[systemName]!.text =
                                    (currentValue > 30 ? currentValue - 1 : 30)
                                        .toString();
                              },
                            ),
                            SizedBox(
                              width: 100,
                              child: TextField(
                                controller: humidityControllers[systemName],
                                keyboardType: TextInputType.number,
                                textAlign: TextAlign.center,
                                decoration:
                                    const InputDecoration(hintText: '30-100%'),
                                onSubmitted: (value) {
                                  int parsedValue = int.tryParse(value) ?? 50;
                                  parsedValue = parsedValue.clamp(30, 100);
                                  humidityControllers[systemName]?.text =
                                      parsedValue.toString();
                                  _saveHumidityLevel(systemName);
                                },
                              ),
                            ),
                            IconButton(
                              icon: const Icon(Icons.add),
                              onPressed: () {
                                int currentValue = int.tryParse(
                                        humidityControllers[systemName]!
                                            .text) ??
                                    50;
                                humidityControllers[systemName]!.text =
                                    (currentValue < 100
                                            ? currentValue + 1
                                            : 100)
                                        .toString();
                              },
                            ),
                            
                            Switch(
                              
                              value: automationHumidityMap[systemName] ?? false,
                              onChanged: (bool value) {
                                setState(() {
                                  automationHumidityMap[systemName] = value;
                                });
                              },
                            ),
                            IconButton(
                              icon: const Icon(Icons.save),
                              onPressed: () {
                                _saveHumidityLevel(systemName);
                                _saveBooleanValueToFirebase(
                                  systemName,
                                  'HumidityControl/automationHumidity',
                                  automationHumidityMap[systemName] ?? false,
                                );
                                // Similarly, save other boolean values here...
                              },
                            ),
                          ],
                        ),
                      ),

                      // pH Balance controls
                      ListTile(
                        leading: const Icon(Icons.spa),
                        title: const Text('pH Balance'),
                        subtitle: Row(
                          children: [
                            IconButton(
                              icon: const Icon(Icons.remove),
                              onPressed: () {
                                double currentValue = double.tryParse(
                                        phMinControllers[systemName]!.text) ??
                                    5.4;
                                phMinControllers[systemName]!.text =
                                    (currentValue > 0 ? currentValue - 0.1 : 0)
                                        .toStringAsFixed(1);
                              },
                            ),
                             Expanded(
                              child: TextField(
                                controller: phMinControllers[systemName],
                                keyboardType: TextInputType.numberWithOptions(decimal: true),
                                textAlign: TextAlign.center,
                                decoration: const InputDecoration(hintText: 'Min pH'),
                                onSubmitted: (value) {
                                  double parsedValue = double.tryParse(value) ?? 5.4;
                                  parsedValue = parsedValue.clamp(0, 14); // pH range is 0-14
                                  phMinControllers[systemName]?.text = parsedValue.toStringAsFixed(1);
                                  _savePHBalance(systemName, 'phMin', parsedValue.toStringAsFixed(1));
                                },
                              ),
                            ),
                           IconButton(
                            icon: const Icon(Icons.add),
                            onPressed: () {
                              double currentValue = double.tryParse(phMinControllers[systemName]!.text) ?? 5.4;
                              phMinControllers[systemName]!.text = (currentValue < 14 ? currentValue + 0.1 : 14).toStringAsFixed(1);
                            },
                          ),
                          // Separator
                          const Text(' - '),
                          // Decrement for Max pH
                          IconButton(
                            icon: const Icon(Icons.remove),
                            onPressed: () {
                              double currentValue = double.tryParse(phMaxControllers[systemName]!.text) ?? 6.4;
                              phMaxControllers[systemName]!.text = (currentValue > 0 ? currentValue - 0.1 : 0).toStringAsFixed(1);
                            },
                          ),
                          // TextField for Max pH
                          Expanded(
                            child: TextField(
                              controller: phMaxControllers[systemName],
                              keyboardType: TextInputType.numberWithOptions(decimal: true),
                              textAlign: TextAlign.center,
                              decoration: const InputDecoration(hintText: 'Max pH'),
                              onSubmitted: (value) {
                                double parsedValue = double.tryParse(value) ?? 6.4;
                                parsedValue = parsedValue.clamp(0, 14); // pH range is 0-14
                                phMaxControllers[systemName]?.text = parsedValue.toStringAsFixed(1);
                                _savePHBalance(systemName, 'phMax', parsedValue.toStringAsFixed(1));
                              },
                            ),
                          ),
                          // Increment for Max pH
                          IconButton(
                            icon: const Icon(Icons.add),
                            onPressed: () {
                              double currentValue = double.tryParse(phMaxControllers[systemName]!.text) ?? 6.4;
                              phMaxControllers[systemName]!.text = (currentValue < 14 ? currentValue + 0.1 : 14).toStringAsFixed(1);
                            },
                          ),
                          // pH Auto Switch
                          Switch(
                            value: phAutoMap[systemName] ?? false,
                            onChanged: (bool value) {
                              setState(() {
                                phAutoMap[systemName] = value;
                              });
                            },
                          ),
                          // Save Button
                          IconButton(
                            icon: const Icon(Icons.save),
                            onPressed: () {
                              _savePHBalance(systemName, 'phMin', phMinControllers[systemName]!.text);
                              _savePHBalance(systemName, 'phMax', phMaxControllers[systemName]!.text);
                              _saveBooleanValueToFirebase(
                                systemName,
                                'phBalance/phAuto',
                                phAutoMap[systemName] ?? false,
                              );
                            },
                          ),
                        ],
                      ),
                    ),
                      // InitialDose widgets
                      ListTile(
                        leading: const Icon(Icons.opacity),
                        title: const Text('Initial Dose - Solution A'),
                        subtitle: Row(
                          children: [
                            IconButton(
                              icon: const Icon(Icons.remove),
                              onPressed: () {
                                double currentValue = double.tryParse(
                                        solutionAControllers[systemName]!
                                            .text) ??
                                    0;
                                solutionAControllers[systemName]!.text =
                                    (currentValue - 0.1).toStringAsFixed(1);
                              },
                            ),
                            SizedBox(
                              width: 100,
                              child: TextField(
                                controller: solutionAControllers[systemName],
                                keyboardType: TextInputType.numberWithOptions(
                                    decimal: true),
                                textAlign: TextAlign.center,
                                decoration: const InputDecoration(
                                    hintText: 'Solution A dose'),
                              ),
                            ),
                            IconButton(
                              icon: const Icon(Icons.add),
                              onPressed: () {
                                double currentValue = double.tryParse(
                                        solutionAControllers[systemName]!
                                            .text) ??
                                    0;
                                solutionAControllers[systemName]!.text =
                                    (currentValue + 0.1).toStringAsFixed(1);
                              },
                            ),
                            IconButton(
                              icon: const Icon(Icons.save),
                              onPressed: () {
                                _saveValueToFirebase(
                                    systemName,
                                    'InitialDose/Amounts/A',
                                    solutionAControllers[systemName]!.text);
                              },
                            ),
                          ],
                        ),
                      ),

                      ListTile(
                        leading: const Icon(Icons.opacity),
                        title: const Text('Initial Dose - Solution B'),
                        subtitle: Row(
                          children: [
                            IconButton(
                              icon: const Icon(Icons.remove),
                              onPressed: () {
                                double currentValue = double.tryParse(
                                        solutionBControllers[systemName]!
                                            .text) ??
                                    0;
                                solutionBControllers[systemName]!.text =
                                    (currentValue - 0.1).toStringAsFixed(1);
                              },
                            ),
                            SizedBox(
                              width: 100,
                              child: TextField(
                                controller: solutionBControllers[systemName],
                                keyboardType: TextInputType.numberWithOptions(
                                    decimal: true),
                                textAlign: TextAlign.center,
                                decoration: const InputDecoration(
                                    hintText: 'Solution B dose'),
                              ),
                            ),
                            IconButton(
                              icon: const Icon(Icons.add),
                              onPressed: () {
                                double currentValue = double.tryParse(
                                        solutionBControllers[systemName]!
                                            .text) ??
                                    0;
                                solutionBControllers[systemName]!.text =
                                    (currentValue + 0.1).toStringAsFixed(1);
                              },
                            ),
                            IconButton(
                              icon: const Icon(Icons.save),
                              onPressed: () {
                                _saveValueToFirebase(
                                    systemName,
                                    'InitialDose/Amounts/B',
                                    solutionBControllers[systemName]!.text);
                              },
                            ),
                          ],
                        ),
                      ),

                      ListTile(
                        leading: const Icon(Icons.settings_input_component),
                        title: const Text('System Volume'),
                        subtitle: Row(
                          children: [
                            IconButton(
                              icon: const Icon(Icons.remove),
                              onPressed: () {
                                double currentValue = double.tryParse(
                                        systemVolumeControllers[systemName]!
                                            .text) ??
                                    0;
                                systemVolumeControllers[systemName]!.text =
                                    (currentValue - 0.1).toStringAsFixed(1);
                              },
                            ),
                            SizedBox(
                              width: 100,
                              child: TextField(
                                controller: systemVolumeControllers[systemName],
                                keyboardType: TextInputType.numberWithOptions(
                                    decimal: true),
                                textAlign: TextAlign.center,
                                decoration: const InputDecoration(
                                    hintText: 'System volume'),
                              ),
                            ),
                            IconButton(
                              icon: const Icon(Icons.add),
                              onPressed: () {
                                double currentValue = double.tryParse(
                                        systemVolumeControllers[systemName]!
                                            .text) ??
                                    0;
                                systemVolumeControllers[systemName]!.text =
                                    (currentValue + 0.1).toStringAsFixed(1);
                              },
                            ),
                            Switch(
                              value: startInitialDosingMap[systemName] ?? false,
                              onChanged: (bool value) {
                                setState(() {
                                  startInitialDosingMap[systemName] = value;
                                });
                              },
                            ),
                            IconButton(
                              icon: const Icon(Icons.save),
                              onPressed: () {
                                _saveValueToFirebase(
                                    systemName,
                                    'InitialDose/systemVolume',
                                    systemVolumeControllers[systemName]!.text);
                                _saveBooleanValueToFirebase(
                                  systemName,
                                  'InitialDose/startInitialDosing',
                                  startInitialDosingMap[systemName] ?? false,
                                );
                              },
                            ),
                          ],
                        ),
                      ),
                      ListTile(
                        leading: const Icon(Icons.lightbulb_outline),
                        title: const Text('Light Control'),
                        subtitle: Row(
                          children: [
                            Expanded(
                              flex: 2, // Adjust the flex factor to give more space to the text fields if needed
                              child: TextField(
                                controller: lightOnControllers[systemName] ??=
                                    TextEditingController(
                                        text: systemData['LightControl']?['lightON']?.toString() ?? ''),
                                keyboardType: TextInputType.datetime,
                                textAlign: TextAlign.center,
                                decoration: const InputDecoration(labelText: 'Start Time'),
                              ),
                            ),
                            const SizedBox(width: 10),
                            Expanded(
                              flex: 2, 
                                child: TextField(
                                controller: lightOffControllers[systemName] ??=
                                    TextEditingController(
                                        text: systemData['LightControl']
                                                    ?['lightOFF']
                                                ?.toString() ??
                                            ''),
                                keyboardType: TextInputType.datetime,
                                textAlign: TextAlign.center,
                                decoration: const InputDecoration(
                                    labelText: 'End Time'),
                              ),
                            ),
                             Expanded(
                               flex: 1, // Adjust the space for the switch
                               child: Switch(
                              value: lightAutoMap[systemName] ?? false,
                              onChanged: (bool value) {
                                setState(() {
                                  lightAutoMap[systemName] = value;
                                });
                              },
                            ),
                             ),
                           IconButton(
                            icon: const Icon(Icons.save),
                            onPressed: () {
                              final String startTime = lightOnControllers[systemName]?.text ?? '00:00';
                              final String endTime = lightOffControllers[systemName]?.text ?? '00:00';
                              _saveLightControl(systemName, startTime, endTime);
                              _saveBooleanValueToFirebase(
                                systemName,
                                'LightControl/lightAuto',
                                lightAutoMap[systemName] ?? false,
                              );
                            },
                          ),
                        ],
                      ),
                    ),
                    ],
                  );
                } else {
                  return ListTile(
                    title: Text('System $systemName'),
                    subtitle: const Text(
                        'System data not found or is not in expected format'),
                  );
                }
              }).toList();

              return DefaultTabController(
                length: systemsData.length,
                child: Scaffold(
                  body: Column(
                    children: [
                      TabBar(
                        isScrollable: true,
                        tabs: systemsData.keys
                            .map(
                                (systemName) => Tab(text: 'System $systemName'))
                            .toList(),
                      ),
                      Expanded(
                        child: TabBarView(
                          children: systemWidgets,
                        ),
                      ),
                    ],
                  ),
                ),
              );
            } else {
              return const Text(
                  'Data structure does not match expected format');
            }
          }
        },
      ),
    );
  }
}