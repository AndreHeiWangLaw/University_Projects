import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'overview.dart';

class Account_settings extends StatefulWidget {
  const Account_settings({Key? key}) : super(key: key);

  @override
  State<Account_settings> createState() => _Account_settingsState();
}

class _Account_settingsState extends State<Account_settings> {
  final TextEditingController _nameController = TextEditingController();
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();

  final FirebaseAuth _auth = FirebaseAuth.instance;

  @override
  void initState() {
    super.initState();
    final User? user = _auth.currentUser;
    if (user != null) {
      // Initialize the text field with the current display name and email
      _nameController.text = user.displayName ?? '';
      _emailController.text = user.email ?? '';
    }
  }

  Future<void> _updateUserProfile() async {
  User? user = _auth.currentUser;
  String newName = _nameController.text.trim();
  String newEmail = _emailController.text.trim();
  String newPassword = _passwordController.text.trim();

  try {
    if (user != null) {
      bool needToUpdate = false;

      // Update the user's display name
      if (newName.isNotEmpty && newName != user.displayName) {
        await user.updateDisplayName(newName);
        needToUpdate = true;
      }

      // Update the user's email
      if (newEmail.isNotEmpty && newEmail != user.email) {
        await user.updateEmail(newEmail);
        needToUpdate = true;
      }

      // Update the user's password
      if (newPassword.isNotEmpty) {
        await user.updatePassword(newPassword);
        needToUpdate = true;
      }

      if (needToUpdate) {
        await user.reload(); 
        _showSuccessDialog(); 
      }
    }
  } on FirebaseAuthException catch (e) {
    _showErrorDialog(e.message ?? 'An error occurred. Please try again.');
  }
}

 void _showSuccessDialog() {
  showDialog(
    context: context,
    barrierDismissible: false, 
    builder: (BuildContext context) {
      return AlertDialog(
        title: const Text('Success'),
        content: const Text('Your profile has been successfully updated.'),
        actions: <Widget>[
          TextButton(
            child: const Text('OK'),
            onPressed: () {
              Navigator.of(context).pop(); 
              Navigator.pushReplacement(
                context,
                MaterialPageRoute(builder: (context) => const OverviewPage()),
              ); 
            },
          ),
        ],
      );
    },
  );
}


  void _showErrorDialog(String message) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Error'),
          content: Text(message),
          actions: <Widget>[
            TextButton(
              child: const Text('OK'),
             onPressed: () {
              Navigator.of(context).pop();
             Navigator.push(context, MaterialPageRoute(builder: (context) => const OverviewPage())); 
            },
          ),
        ],
      );
    },
  );
}


@override
Widget build(BuildContext context) {
  return Scaffold(
    appBar: AppBar(title: const Text('Settings')),
    body: Padding(
      padding: const EdgeInsets.all(20.0),
      child: ListView(
        children: [
          TextField(
            controller: _nameController,
            decoration: const InputDecoration(labelText: 'Name'),
          ),
          const SizedBox(height: 20),
          TextField(
            controller: _emailController,
            decoration: const InputDecoration(labelText: 'Email'),
            keyboardType: TextInputType.emailAddress,
          ),
          const SizedBox(height: 20),
          TextField(
            controller: _passwordController,
            decoration: const InputDecoration(labelText: 'New Password'),
            obscureText: true,
          ),
          const SizedBox(height: 30),
          ElevatedButton(
            onPressed: _updateUserProfile,
            child: const Text('Update Profile'),
          ),
        ],
      ),
    ),
  );
}


  @override
  void dispose() {
    _nameController.dispose();
    _emailController.dispose();
    _passwordController.dispose();
    super.dispose();
  }
}