import 'package:flutter/material.dart';
import 'package:flutter_capstone_growpulse/login.dart'; 

class HomePage extends StatelessWidget {
  const HomePage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
       body: Stack(
        fit: StackFit.expand, 
        children: [
          Align(
            alignment: Alignment.center,
            child: Container(
              width: MediaQuery.of(context).size.width * 0.95, 
              height: MediaQuery.of(context).size.height * 0.95,  
              decoration: BoxDecoration(
                image: DecorationImage(
                  image: AssetImage('assets/leaf_transparent.png'),
                  fit: BoxFit.contain, 
                ),
              ),
            ),
          ),
       Center(
        child: Column(
          mainAxisSize: MainAxisSize.min,
          mainAxisAlignment: MainAxisAlignment.center,
           crossAxisAlignment: CrossAxisAlignment.center,
           children: <Widget>[
            Padding( 
              padding: const EdgeInsets.only(top: 40.0, left: 40.0, right: 40.0),
              child: Image.asset('assets/growpulse-align-left.png', width: 300, height: 300),
            ),
            const SizedBox(height: 40), 
            ElevatedButton(
              style: ButtonStyle(
                foregroundColor: MaterialStateProperty.all<Color>(Colors.blue),
                backgroundColor: MaterialStateProperty.all<Color>(Colors.white),
                overlayColor: MaterialStateProperty.all<Color>(Colors.blueAccent),
                shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                  RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(20.0),
                    side: const BorderSide(color: Colors.blue),
                  ),
                ),
              ),
              child: const Text('Login',style: TextStyle(fontSize: 20, color: Colors.blue)),
              onPressed: () {
                Navigator.push(context, MaterialPageRoute(builder: (context) => const LoginScreen()));
              },
            ),
            const SizedBox(height: 24), // Space between buttons
            ElevatedButton(
              style: ButtonStyle(
                foregroundColor: MaterialStateProperty.all<Color>(Colors.blue),
                backgroundColor: MaterialStateProperty.all<Color>(Colors.white),
                overlayColor: MaterialStateProperty.all<Color>(Colors.blueAccent),
                shape: MaterialStateProperty.all<RoundedRectangleBorder>(
                  RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(20.0),
                    side: const BorderSide(color: Colors.blue),
                  ),
                ),
              ),
              child: const Text('Sign Up',style: TextStyle(fontSize: 20, color: Colors.blue)),
              onPressed: () {
                Navigator.pushNamed(context, '/signup');
              }
              ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}