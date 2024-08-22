package com.hydroponics.hydromind;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class Settings extends AppCompatActivity {

    private TextView fullNameTextView, emailTextView;
    private ProgressBar progressBar;
    private String fullName,email;
    private FirebaseAuth authProfile;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);
        getSupportActionBar().setTitle("Settings");

        fullNameTextView = findViewById(R.id.fullName);
        emailTextView = findViewById(R.id.email);
        progressBar = findViewById(R.id.progressBar);

        Button signOut = findViewById(R.id.signOut);

        authProfile = FirebaseAuth.getInstance();
        FirebaseUser firebaseUser = authProfile.getCurrentUser();

        if (firebaseUser == null){
            Toast.makeText(this, "Something went Wrong! User details are not available at the moment! Please try again later.", Toast.LENGTH_SHORT).show();
        } else {
            progressBar.setVisibility(View.VISIBLE);
            showUserProfile(firebaseUser);
        }

        signOut.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                authProfile.signOut();
                openMain();
            }
        });

    }

    private void openMain() {
        Intent intent = new Intent(Settings.this,MainActivity.class);
        startActivity(intent);
    }

    private void showUserProfile(FirebaseUser firebaseUser) {
        String userID = firebaseUser.getUid();

        DatabaseReference referenceProfile = FirebaseDatabase.getInstance().getReference("Registered Users");
        referenceProfile.child(userID).addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                fullName = firebaseUser.getDisplayName();
                email = firebaseUser.getEmail();

                fullNameTextView.setText(fullName);
                emailTextView.setText(email);

                progressBar.setVisibility(View.GONE);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(Settings.this, "Signed Out!", Toast.LENGTH_SHORT).show();
                progressBar.setVisibility(View.GONE);
            }
        });
    }

}