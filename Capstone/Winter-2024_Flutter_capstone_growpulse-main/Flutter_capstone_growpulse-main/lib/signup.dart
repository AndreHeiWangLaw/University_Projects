import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter_capstone_growpulse/login.dart';

class SignUpScreen extends StatefulWidget {
  const SignUpScreen({Key? key}) : super(key: key);

  @override
  _SignUpScreenState createState() => _SignUpScreenState();
}

class _SignUpScreenState extends State<SignUpScreen> {
  final TextEditingController _fullNameController = TextEditingController();
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();

  @override
  void dispose() {
    _fullNameController.dispose();
    _emailController.dispose();
    _passwordController.dispose();
    super.dispose();
  }

InputDecoration _buildInputDecoration({required IconData icon, required String label}) {
    return InputDecoration(
      labelText: label,
      hintText: 'Enter your $label',
      prefixIcon: Icon(icon, color: Colors.blue),
      border: OutlineInputBorder(
        borderRadius: BorderRadius.circular(5.0),
        borderSide: BorderSide(color: Colors.blue),
      ),
      enabledBorder: OutlineInputBorder(
        borderRadius: BorderRadius.circular(5.0),
        borderSide: BorderSide(color: Colors.blue),
      ),
      focusedBorder: OutlineInputBorder(
        borderRadius: BorderRadius.circular(5.0),
        borderSide: BorderSide(color: Colors.blue, width: 2),
      ),
    );
  }


  void _registerAccount() async {
    final String fullName = _fullNameController.text.trim();
    final String email = _emailController.text.trim();
    final String password = _passwordController.text.trim();

    if (fullName.isEmpty || email.isEmpty || password.isEmpty) {
      _showErrorDialog('All fields are required.');
      return;
    }

    try {
      // Create the user account with email and password
      UserCredential userCredential = await FirebaseAuth.instance.createUserWithEmailAndPassword(
        email: email,
        password: password,
      );

      User? user = userCredential.user;
      if (user != null) {
        // Send email verification
        await user.sendEmailVerification();

        // Write to the Realtime Database
        DatabaseReference userRef = FirebaseDatabase.instance.ref('Registered Users/${user.uid}');
        await userRef.set({
          'full_name': fullName,
          'email': email,
        });

        // Show a success message or navigate to a different screen
        _showVerificationEmailSentDialog(email);
      }
    } on FirebaseAuthException catch (e) {
      if (e.code == 'weak-password') {
        _showErrorDialog('The password provided is too weak.');
      } else if (e.code == 'email-already-in-use') {
        _showErrorDialog('The account already exists for that email.');
      } else {
        _showErrorDialog(e.message ?? 'An error occurred. Please try again later.');
      }
    } catch (e) {
      _showErrorDialog('An error occurred. Please try again later.');
    }
  }

  void _showVerificationEmailSentDialog(String email) {
    showDialog(
      context: context,
      builder: (ctx) => AlertDialog(
        title: const Text('Verify Your Email'),
        content: Text('A verification email has been sent to $email. Please check your email and verify your account.Then login'),
        actions: <Widget>[
          TextButton(
            child: const Text('Okay'),
            onPressed: () {
              Navigator.of(ctx).pop();
             Navigator.push(context, MaterialPageRoute(builder: (context) => const LoginScreen()));
            },
          )
        ],
      ),
    );
  }

  void _showErrorDialog(String message) {
    showDialog(
      context: context,
      builder: (ctx) => AlertDialog(
        title: const Text('Registration Error'),
        content: Text(message),
        actions: <Widget>[
          TextButton(
            child: const Text('Okay'),
            onPressed: () {
              Navigator.of(ctx).pop();
            },
          )
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final ButtonStyle outlineButtonStyle = ButtonStyle(
      foregroundColor: MaterialStateProperty.all<Color>(Colors.blue),
      backgroundColor: MaterialStateProperty.all<Color>(Colors.white),
      side: MaterialStateProperty.all<BorderSide>(const BorderSide(color: Colors.blue)),
      overlayColor: MaterialStateProperty.all<Color>(Colors.blueAccent),
      shape: MaterialStateProperty.all<RoundedRectangleBorder>(
        RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(20.0),
        ),
      ),
      padding: MaterialStateProperty.all<EdgeInsets>(EdgeInsets.symmetric(vertical: 10.0, horizontal: 20.0)),
    );
    final EdgeInsets textFieldPadding = const EdgeInsets.symmetric(horizontal: 32.0, vertical: 8.0);

   return Scaffold(
      appBar: AppBar(
        title: const Text('Sign Up'),
      ), body: Stack(
        fit: StackFit.expand,
        children: [
          // Background image
          Align(
            alignment: Alignment.center,
            child: Container(
              // Adjust the scale to your preference
              width: MediaQuery.of(context).size.width * 0.95,  // 90% of the screen width
              height: MediaQuery.of(context).size.height * 0.95,  // 90% of the screen height
              decoration: BoxDecoration(
                image: DecorationImage(
                  image: AssetImage('assets/leaf_transparent.png'),
                  fit: BoxFit.contain,  // Will scale the image proportionally
                ),
              ),
            ),
          ),
          Center(
      child: Padding(
        padding: const EdgeInsets.all(20.0), // Match the padding from LoginScreen
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            // Full Name TextField
            TextField(
              controller: _fullNameController,
              decoration: _buildInputDecoration(icon: Icons.person, label: 'Full Name'),
            ),
            const SizedBox(height: 20.0), // Match the SizedBox height from LoginScreen
            // Email TextField
            TextField(
              controller: _emailController,
              keyboardType: TextInputType.emailAddress,
              decoration: _buildInputDecoration(icon: Icons.email, label: 'Email'),
            ),
            const SizedBox(height: 20.0), // Match the SizedBox height from LoginScreen
            // Password TextField
            TextField(
              controller: _passwordController,
              obscureText: true,
              decoration: _buildInputDecoration(icon: Icons.lock, label: 'Password'),
            ),
            const SizedBox(height: 20.0), // Match the SizedBox height from LoginScreen
            // Register Button
            ElevatedButton.icon(
              icon: Icon(Icons.person_add, color: Colors.blue),
              label: Text('Register', style: TextStyle(fontSize: 20, color: Colors.blue)),
              onPressed: _registerAccount,
              style: outlineButtonStyle,
              ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }
}