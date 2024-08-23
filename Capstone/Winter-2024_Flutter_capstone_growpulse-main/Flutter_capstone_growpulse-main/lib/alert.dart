import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'custom_app_bar.dart';

class MyAlertDialog extends StatefulWidget {
  @override
  _MyAlertDialogState createState() => _MyAlertDialogState();
}

class _MyAlertDialogState extends State<MyAlertDialog> {
  final FirebaseAuth auth = FirebaseAuth.instance;
  final FirebaseDatabase database = FirebaseDatabase.instance;

  User? currentUser;

  @override
  void initState() {
    super.initState();
    currentUser = auth.currentUser;
  }

  Future<void> toggleAlert(String alertPath, bool currentValue) async {
    DatabaseReference alertRef = database.ref(alertPath);
    await alertRef.set(!currentValue);
  }

  Widget buildToggle(String label, bool? isActive, IconData icon,
      String detailText, VoidCallback onToggle) {
    return ListTile(
      leading: Icon(icon),
      title: Text(label),
      subtitle: Text(detailText),
      trailing: Switch(
        value: isActive ?? false,
        onChanged: (bool value) {
          onToggle();
        },
      ),
    );
  }

  String getDetailText(Map<dynamic, dynamic> alertDetails, String alertType) {
    String detailText = '';
    if (alertType == 'Humidity') {
      detailText = 'Offset: ${alertDetails['humidityOffset'] ?? '-'}';
    } else if (alertType == 'TDS') {
      detailText = 'Min: ${alertDetails['tdsMin'] ?? '-'} ppm';
    } else if (alertType == 'AirTemperature') {
      detailText =
          'Min: ${alertDetails['airTempMin'] ?? '-'}°, Max: ${alertDetails['airTempMax'] ?? '-'}°';
    } else if (alertType == 'WaterTemperature') {
      detailText =
          'Min: ${alertDetails['waterTempMin'] ?? '-'}°, Max: ${alertDetails['waterTempMax'] ?? '-'}°';
    }
    return detailText;
  }

  IconData getIcon(String alertType) {
    switch (alertType) {
      case 'CAM':
        return Icons.videocam;
      case 'DP':
        return Icons.opacity;
      case 'Humidity':
        return Icons.grain;
      case 'TDS':
        return Icons.show_chart;
      case 'AirTemperature':
        return Icons.ac_unit;
      case 'WaterTemperature':
        return Icons.invert_colors;
      default:
        return Icons.warning; 
    }
  }

  Widget buildAlertRow(Map<dynamic, dynamic> alerts, String systemName) {
    // Original maps and bool values you provided
    Map<dynamic, dynamic>? airTempAlerts =
        alerts['AirTemperature'] as Map<dynamic, dynamic>?;
    bool airTempAlertValue = airTempAlerts?['airTempAlert'] ?? false;
    Map<dynamic, dynamic>? waterTempAlerts =
        alerts['WaterTemperature'] as Map<dynamic, dynamic>?;
    bool waterTempAlertValue = waterTempAlerts?['waterTempAlert'] ?? false;
    bool camAlertValue = alerts['CAM']?['camAlert'] ?? false;
    bool dpAlertValue = alerts['DP']?['dpAlert'] ?? false;
    bool humidityAlertValue = alerts['Humidity']?['humidityAlert'] ?? false;
    bool tdsAlertValue = alerts['TDS']?['tdsAlert'] ?? false;

    return Card(
      child: ExpansionTile(
        title: Text(systemName),
        children: <Widget>[
          buildToggle(
            'pH Alert',
            alerts['pH']['phAlert'],
            Icons.blur_on,
            '',
            () => toggleAlert(
                'Registered Users/${currentUser!.uid}/$systemName/Alerts/pH/phAlert',
                alerts['pH']['phAlert']),
          ),
          buildToggle(
            'Air Temperature Alert',
            airTempAlertValue,
            getIcon('AirTemperature'),
            getDetailText(airTempAlerts ?? {}, 'AirTemperature'),
            () => toggleAlert(
                'Registered Users/${currentUser!.uid}/$systemName/Alerts/AirTemperature/airTempAlert',
                airTempAlertValue),
          ),
          buildToggle(
            'Water Temperature Alert',
            waterTempAlertValue,
            getIcon('WaterTemperature'),
            getDetailText(waterTempAlerts ?? {}, 'WaterTemperature'),
            () => toggleAlert(
                'Registered Users/${currentUser!.uid}/$systemName/Alerts/WaterTemperature/waterTempAlert',
                waterTempAlertValue),
          ),
          buildToggle(
            'CAM Alert',
            camAlertValue,
            getIcon('CAM'),
            '', 
            () => toggleAlert(
                'Registered Users/${currentUser!.uid}/$systemName/Alerts/CAM/camAlert',
                camAlertValue),
          ),
          buildToggle(
            'DP Alert',
            dpAlertValue,
            getIcon('DP'),
            '', 
            () => toggleAlert(
                'Registered Users/${currentUser!.uid}/$systemName/Alerts/DP/dpAlert',
                dpAlertValue),
          ),
          buildToggle(
            'Humidity Alert',
            humidityAlertValue,
            getIcon('Humidity'),
            getDetailText(alerts['Humidity'] ?? {}, 'Humidity'),
            () => toggleAlert(
                'Registered Users/${currentUser!.uid}/$systemName/Alerts/Humidity/humidityAlert',
                humidityAlertValue),
          ),
          buildToggle(
            'TDS Alert',
            tdsAlertValue,
            getIcon('TDS'),
            getDetailText(alerts['TDS'] ?? {}, 'TDS'),
            () => toggleAlert(
                'Registered Users/${currentUser!.uid}/$systemName/Alerts/TDS/tdsAlert',
                tdsAlertValue),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    if (currentUser == null) {
      return Center(child: Text('Please log in to view alerts'));
    }

    DatabaseReference systemsRef =
        database.ref('Registered Users/${currentUser!.uid}');

    return Scaffold(
     appBar: CustomAppBar(title: 'System Alerts'),
      body: StreamBuilder(
        stream: systemsRef.onValue,
        builder: (context, AsyncSnapshot<DatabaseEvent> snapshot) {
          if (snapshot.hasData &&
              !snapshot.hasError &&
              snapshot.data!.snapshot.value != null) {
            Map<dynamic, dynamic> systemsData =
                snapshot.data!.snapshot.value as Map<dynamic, dynamic>;
            List<Widget> systemWidgets = systemsData.entries.map((entry) {
              if (entry.key.toString().contains('System')) {
                return buildAlertRow(entry.value['Alerts'], entry.key);
              } else {
                return SizedBox
                    .shrink(); 
              }
            }).toList();
            return ListView(
              children: systemWidgets,
            );
          } else {
            return Center(
              child: snapshot.hasError
                  ? Text('Error: ${snapshot.error}')
                  : CircularProgressIndicator(),
            );
          }
        },
      ),
    );
  }
}