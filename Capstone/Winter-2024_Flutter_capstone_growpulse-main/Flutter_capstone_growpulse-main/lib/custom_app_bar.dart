import 'package:flutter/material.dart';
import 'package:firebase_auth/firebase_auth.dart';

class CustomAppBar extends StatelessWidget implements PreferredSizeWidget {
  final String title;
  final bool showLeading;

  const CustomAppBar({Key? key, this.title = 'Growpulse', this.showLeading = true}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return AppBar(
      title: Text(title),
       leading: showLeading ? IconButton(
        icon: Icon(Icons.arrow_back),
        onPressed: () => Navigator.of(context).pop(),
      ) : null,
      actions: <Widget>[
        PopupMenuButton<String>(
          icon: const Icon(Icons.settings),
          onSelected: (String result) {
            switch (result) {
              case 'settings':
                Navigator.pushNamed(context, '/settings');
                break;
              case 'logout':
                _logout(context);
                break;
            }
          },
          itemBuilder: (BuildContext context) => <PopupMenuEntry<String>>[
            const PopupMenuItem<String>(
              value: 'settings',
              child: Text('Account Settings'),
            ),
            const PopupMenuItem<String>(
              value: 'logout',
              child: Text('Logout'),
            ),
          ],
        ),
      ],
    );
  }

  void _logout(BuildContext context) async {
    await FirebaseAuth.instance.signOut();
    Navigator.of(context).pushNamedAndRemoveUntil('/home', (Route<dynamic> route) => false);
  }

  @override
  Size get preferredSize => const Size.fromHeight(kToolbarHeight);
}
