import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'custom_app_bar.dart';

void main() {
  runApp(MaterialApp(
    home: DispensedPage(),
  ));
}

class DispensedPage extends StatelessWidget {
  const DispensedPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 2,
      child: Scaffold(
        appBar: CustomAppBar(title: 'Dispensed'), 
        body: Column(
          children: [
            TotalDispensedWidget(),
            TabBar(
              tabs: [
                Tab(text: 'System A'),
                Tab(text: 'System B'),
              ],
            ),
            Expanded(
              child: TabBarView(
                children: [
                  DispenseHistoryWidget(system: 'SystemA'),
                  DispenseHistoryWidget(system: 'SystemB'),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class DispenseHistoryWidget extends StatelessWidget {
  final String system;

  const DispenseHistoryWidget({Key? key, required this.system}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Column(
      crossAxisAlignment: CrossAxisAlignment.stretch,
      children: [
        DispenseSystemWidget(system: system),
        SizedBox(height: 20.0),
      ],
    );
  }
}

class DispenseSystemWidget extends StatelessWidget {
  final String system;

  const DispenseSystemWidget({Key? key, required this.system}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return StreamBuilder<DocumentSnapshot>(
      stream: FirebaseFirestore.instance
          .collection('Registered Users')
          .doc(FirebaseAuth.instance.currentUser!.uid)
          .collection(system)
          .doc('DispenseHistory')
          .snapshots(),
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.waiting) {
          return Center(child: CircularProgressIndicator());
        } else if (snapshot.hasError) {
          return Center(child: Text('Error: ${snapshot.error}'));
        } else if (!snapshot.hasData || !snapshot.data!.exists) {
          return Center(child: Text('No data found for $system'));
        }

        final dispenseHistory = snapshot.data!.data() as Map<String, dynamic>;

        final totalA = dispenseHistory['totalA'] ?? 0;
        final totalB = dispenseHistory['totalB'] ?? 0;
        final totalDown = dispenseHistory['totalDown'] ?? 0;
        final totalUp = dispenseHistory['totalUp'] ?? 0;
          
          return Card(
          color: Colors.white,
          elevation: 2.0,
          margin: const EdgeInsets.all(12.0), // Adjust the margin as needed
          child: Padding(
            padding: const EdgeInsets.symmetric(vertical: 20.0, horizontal: 16.0), // Adjust the padding as needed
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text('Total A: $totalA', style: TextStyle(fontSize: 16.0)),
                SizedBox(height: 4.0),
                Text('Total B: $totalB', style: TextStyle(fontSize: 16.0)),
                SizedBox(height: 4.0),
                Text('Total Down: $totalDown', style: TextStyle(fontSize: 16.0)),
                SizedBox(height: 4.0),
                Text('Total Up: $totalUp', style: TextStyle(fontSize: 16.0)),
              ],
            ),
          ),
        );
      },
    );
  }
}

class TotalDispensedWidget extends StatelessWidget {
  const TotalDispensedWidget({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    final String uid = FirebaseAuth.instance.currentUser!.uid;

    return StreamBuilder<QuerySnapshot>(
      stream: FirebaseFirestore.instance
          .collection('Registered Users')
          .doc(uid)
          .collection('SystemA')
          .snapshots(),
      builder: (context, snapshotA) {
        if (snapshotA.connectionState == ConnectionState.waiting) {
          return Center(child: CircularProgressIndicator());
        } else if (snapshotA.hasError) {
          return Center(child: Text('Error: ${snapshotA.error}'));
        } else if (!snapshotA.hasData) {
          return Center(child: Text('No data found for System A'));
        }

        final systemAData = snapshotA.data!.docs.map((doc) => doc.data()).toList();

        return StreamBuilder<QuerySnapshot>(
          stream: FirebaseFirestore.instance
              .collection('Registered Users')
              .doc(uid)
              .collection('SystemB')
              .snapshots(),
          builder: (context, snapshotB) {
            if (snapshotB.connectionState == ConnectionState.waiting) {
              return Center(child: CircularProgressIndicator());
            } else if (snapshotB.hasError) {
              return Center(child: Text('Error: ${snapshotB.error}'));
            } else if (!snapshotB.hasData) {
              return Center(child: Text('No data found for System B'));
            }

            final systemBData = snapshotB.data!.docs.map((doc) => doc.data()).toList();

            // Calculate totals
            double totalA = 0;
            double totalB = 0;
            double totalDown = 0;
            double totalUp = 0;

            for (var data in systemAData) {
              if (data != null && data is Map<String, dynamic>) {
                totalA += data['totalA'] ?? 0;
                totalB += data['totalB'] ?? 0;
                totalDown += data['totalDown'] ?? 0;
                totalUp += data['totalUp'] ?? 0;
              }
            }

            for (var data in systemBData) {
              if (data != null && data is Map<String, dynamic>) {
                totalA += data['totalA'] ?? 0;
                totalB += data['totalB'] ?? 0;
                totalDown += data['totalDown'] ?? 0;
                totalUp += data['totalUp'] ?? 0;
              }
            }

            int totalAInt = totalA.toInt();
            int totalBInt = totalB.toInt();
            int totalDownInt = totalDown.toInt();
            int totalUpInt = totalUp.toInt();

            return Card(
              elevation: 3.0,
              margin: const EdgeInsets.symmetric(vertical: 10.0),
              child: Padding(
                padding: const EdgeInsets.all(20.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    Text(
                      'Total Amount Dispensed for All Systems',
                      style: TextStyle(fontSize: 18.0, fontWeight: FontWeight.bold),
                    ),
                    SizedBox(height: 10.0),
                    Text('Total A: $totalAInt'),
                    Text('Total B: $totalBInt'),
                    Text('Total Down: $totalDownInt'),
                    Text('Total Up: $totalUpInt'),
                  ],
                ),
              ),
            );
          },
        );
      },
    );
  }
}