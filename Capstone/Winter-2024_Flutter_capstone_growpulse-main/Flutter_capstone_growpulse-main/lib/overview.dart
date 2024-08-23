import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:url_launcher/url_launcher.dart';
import 'custom_app_bar.dart';

class OverviewPage extends StatefulWidget {
  const OverviewPage({Key? key}) : super(key: key);

  @override
  _OverviewPageState createState() => _OverviewPageState();
}

class _OverviewPageState extends State<OverviewPage> {
  bool hasSystem = false;
  bool isLoading = true;

  Future<bool> _onWillPop() async {
    // Return false to prevent the route from being popped
    return false;
  }

  @override
  void initState() {
    super.initState();
    checkForSystem();
  }

  void checkForSystem() async {
    final User? user = FirebaseAuth.instance.currentUser;
    if (user != null) {
      DatabaseReference userRef = FirebaseDatabase.instance.ref('Registered Users/${user.uid}');
      DatabaseEvent event = await userRef.once();
      DataSnapshot snapshot = event.snapshot;

      Map? userData = snapshot.value as Map?;

      if (snapshot.exists && userData != null && userData['SystemA'] != null) {
        setState(() {
          hasSystem = true;
          isLoading = false;
        });
      } else {
        setState(() {
          hasSystem = false;
          isLoading = false;
        });
        showSystemCreationPrompt(context); // Show popup if system does not exist
      }
    }
  }

  void showSystemCreationPrompt(BuildContext context) {
    showDialog(
      context: context,
      barrierDismissible: false, // User must tap button to close dialog
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text('Create a System'),
          content: SingleChildScrollView(
            child: ListBody(
              children: <Widget>[
                Text('Please log in to your account at www.growpulse.live and create a system.'),
                SizedBox(height: 10),
                GestureDetector(
                  onTap: () async {
                    const url = 'https://www.growpulse.live';
                    if (await canLaunch(url)) {
                      await launch(url);
                    } else {
                      ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(
                          content: Text('Could not open the website'),
                        ),
                      );
                    }
                  },
                  child: Text(
                    'Visit www.growpulse.live to get started.',
                    style: TextStyle(
                      color: Colors.blueAccent,
                      decoration: TextDecoration.underline,
                      fontWeight: FontWeight.bold,
                      fontSize: 16,
                    ),
                    textAlign: TextAlign.center,
                  ),
                ),
              ],
            ),
          ),
          actions: <Widget>[
            TextButton(
              onPressed: () {
                Navigator.of(context).pushNamedAndRemoveUntil('/home', (Route<dynamic> route) => false);
              },
              child: Text('Ok'),
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    if (isLoading) {
      return Scaffold(
        body: Center(child: CircularProgressIndicator()),
      );
    }

    if (!hasSystem) {
      return SizedBox.shrink();
    }

    final ButtonStyle buttonStyle = ElevatedButton.styleFrom(
      foregroundColor: Colors.white,
      backgroundColor: Colors.blue,
      minimumSize: Size(240, 50),
      padding: EdgeInsets.symmetric(horizontal: 16),
    );

    return WillPopScope(
      onWillPop: _onWillPop,
      child: Scaffold(
        appBar: CustomAppBar(title: 'Overview'),
        body: Stack(
          children: [
            Container(
              decoration: BoxDecoration(
                image: DecorationImage(
                  image: AssetImage("assets/website_bg.png"),
                  fit: BoxFit.cover,
                ),
              ),
            ),
            Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: <Widget>[
                  ElevatedButton(
                    style: buttonStyle,
                    child: const Text('Control Panel'),
                    onPressed: () {
                      Navigator.pushNamed(context, '/controlpanel');
                    },
                  ),
                  const SizedBox(height: 20),
                  ElevatedButton(
                    style: buttonStyle,
                    child: const Text('Dashboard'),
                    onPressed: () {
                      Navigator.pushNamed(context, '/dashboard');
                    },
                  ),
                  const SizedBox(height: 20),
                  ElevatedButton(
                    style: buttonStyle,
                    child: const Text('Dispensed'),
                    onPressed: () {
                      Navigator.pushNamed(context, '/dispensed');
                    },
                  ),
                  const SizedBox(height: 20),
                  ElevatedButton(
                    style: buttonStyle,
                    child: const Text('Alert'),
                    onPressed: () {
                      Navigator.pushNamed(context, '/alert');
                    },
                  ),
                  const SizedBox(height: 20),
                  ElevatedButton(
                    style: buttonStyle,
                    child: const Text('Information'),
                    onPressed: () {
                      Navigator.pushNamed(context, '/information');
                    },
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}