import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:flutter_capstone_growpulse/account_settings.dart';
import 'firebase_options.dart';
import 'package:flutter_capstone_growpulse/homepage.dart';
import 'package:flutter_capstone_growpulse/login.dart';
import 'package:flutter_capstone_growpulse/signup.dart';
import 'package:flutter_capstone_growpulse/overview.dart';
import 'package:flutter_capstone_growpulse/controlpanel.dart'; 
import 'package:flutter_capstone_growpulse/dashboard.dart'; 
import 'package:flutter_capstone_growpulse/dispensed.dart'; 
import 'alert.dart'; 
import 'information.dart';


void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});


  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Flutter Capstone Growpulse',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const HomePage(),
      routes: {
        '/home': (context) => const HomePage(),
        '/login': (context) => const LoginScreen(),
        '/signup': (context) => const SignUpScreen(),
        '/overview': (context) => const OverviewPage(),
        '/controlpanel': (context) => const ControlPanelPage(), 
        '/dashboard': (context) => const DashboardPage(), 
        '/dispensed': (context) => const DispensedPage(), 
        '/settings':(context) => const Account_settings(),
        '/alert':(context) => MyAlertDialog(),
        '/information':(context) => const InformationPage(),
      },
    );
  }
}

