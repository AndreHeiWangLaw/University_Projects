import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:firebase_storage/firebase_storage.dart';
import 'custom_app_bar.dart';
import 'package:material_design_icons_flutter/material_design_icons_flutter.dart';


void main() {
  runApp(MaterialApp(
    home: DashboardPage(),
  ));
}

class DashboardPage extends StatelessWidget {
  final User? user;

  const DashboardPage({Key? key, this.user}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final DatabaseReference databaseReference = FirebaseDatabase.instance.reference();

    return DefaultTabController(
      length: 2,
      child: Scaffold(
        appBar: CustomAppBar(title: 'Dashboard'), 
        body: Column(
          children: [
            TabBar(
              tabs: [
                Tab(text: 'System A'),
                Tab(text: 'System B'),
              ],
            ),
            Expanded(
              child: TabBarView(
                children: [
                  SystemDataWidget(system: 'SystemA', databaseReference: databaseReference),
                  SystemDataWidget(system: 'SystemB', databaseReference: databaseReference),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class SystemDataWidget extends StatelessWidget {
  final String system;
  final DatabaseReference databaseReference;

  const SystemDataWidget({Key? key, required this.system, required this.databaseReference}) : super(key: key);

  Widget _buildValueWithIcon(IconData icon, String label, String value) {
      if (value == 'N/A') return const SizedBox();
      return Padding(
        padding: const EdgeInsets.symmetric(vertical: 4.0),
        child: Row(
          children: [
            Icon(icon, size: 20.0), 
            const SizedBox(width: 10.0), 
            Expanded(child: Text('$label: $value')), 
          ],
        ),
      );
    }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(20.0),
      child: StreamBuilder(
        stream: databaseReference.child('Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$system/LiveFeed').onValue,
        builder: (context, snapshot) {
          if (snapshot.connectionState == ConnectionState.waiting) {
            return const CircularProgressIndicator();
          } else if (snapshot.hasError) {
            return Text('Error: ${snapshot.error}');
          } else if (!snapshot.hasData || snapshot.data!.snapshot.value == null) {
            return const Text('No data found for this system');
          } else {
            final liveFeeds = snapshot.data!.snapshot.value as Map<dynamic, dynamic>?;

            if (liveFeeds == null) {
              return const Text('No live feed data available');
            }

            final String phValue = liveFeeds['phValue']?.toString() ?? 'N/A';
            final String tdsValue = liveFeeds['tdsValue']?.toString() ?? 'N/A';
            final String waterTempValue = liveFeeds['WaterTemperature']?.toString() ?? 'N/A';
            final String airTempValue = liveFeeds['AirTemperature']?.toString() ?? 'N/A';
            final String humidityValue = liveFeeds['Humidity']?.toString() ?? 'N/A';

            // Fetching ML Results
            final DatabaseReference mlReference = FirebaseDatabase.instance.reference();
            return Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: <Widget>[// Adding ML Prediction Section
              Text(
            'Details',
            style: Theme.of(context).textTheme.headline6?.copyWith(color: Colors.blueAccent),
              ),
              const Divider(color: Colors.blueAccent),
                MLResultWidget(system: system, mlReference: FirebaseDatabase.instance.reference()), // Existing MLResultWidget
              _buildValueWithIcon(Icons.spa, 'pH Value', phValue), // pH 
              _buildValueWithIcon(MdiIcons.water, 'TDS Value', tdsValue), // TDS 
              _buildValueWithIcon(MdiIcons.thermometer, 'Water Temperature', waterTempValue), // Water Temperature 
              _buildValueWithIcon(MdiIcons.weatherWindy, 'Air Temperature', airTempValue), // Air Temperature 
              _buildValueWithIcon(Icons.grain, 'Humidity', humidityValue), // Humidity 
              const SizedBox(height: 20),
              ],
            );
          }
        },
      ),
    );
  }
}

class MLResultWidget extends StatelessWidget {
  final String system;
  final DatabaseReference mlReference;

  const MLResultWidget({Key? key, required this.system, required this.mlReference}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final imageUrl =
        'gs://growpulse-d7198.appspot.com/Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$system/placeHolder.jpeg';

    return FutureBuilder(
      future: _getImage(imageUrl),
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return const CircularProgressIndicator();
        } else if (snapshot.hasError) {
          return Text('Error: ${snapshot.error}');
        } else if (snapshot.hasData) {
          final String? imageUrl = snapshot.data as String?;
          return Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: <Widget>[
              Card(
            elevation: 2,
            clipBehavior: Clip.antiAlias,
            shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(8)),
            child: Image.network(
              imageUrl ?? '',
              width: double.infinity, // Takes full width of the card
              height: 300, // Fixed height
              fit: BoxFit.cover, // Cover the card's area
            ),
          ),
              const SizedBox(height: 10),
              // fetch ML Results from Firebase and display them
              // assuming the prediction is fetched from Firebase Realtime Database
             StreamBuilder(
            stream: mlReference.child('Registered Users/${FirebaseAuth.instance.currentUser!.uid}/$system/mlResults').onValue,
            builder: (context, snapshot) {
              if (snapshot.connectionState == ConnectionState.waiting) {
                return const CircularProgressIndicator();
              } else if (snapshot.hasError) {
                return Text('Error: ${snapshot.error}');
              } else if (!snapshot.hasData || snapshot.data!.snapshot.value == null) {
                return const Text('No ML data found for this system');
              } else {
                final mlResults = snapshot.data!.snapshot.value as Map<dynamic, dynamic>?;

                if (mlResults != null && mlResults.containsKey('prediction')) {
                  final prediction = mlResults['prediction'].toString();
                  // This is where you place the Padding and Text widget for the prediction.
                  return Padding(
                    padding: const EdgeInsets.symmetric(vertical: 8.0),
                    child: Text(
                      'Prediction: $prediction',
                      style: Theme.of(context).textTheme.subtitle1?.copyWith(fontWeight: FontWeight.bold),
                    ),
                  );
                } else {
                  return const Text('Prediction not available');
                }
              }
            },
          ),
            ],
          );
        } else {
          return const Text('Image not available, unavailable to perform prediction');
        }
      },
    );
  }

  Future<String?> _getImage(String imageUrl) async {
    final ref = FirebaseStorage.instance.refFromURL(imageUrl);

    try {
      final url = await ref.getDownloadURL();
      return url;
    } catch (e) {
      print('Error getting image URL: $e');
      return null;
    }
  }
}