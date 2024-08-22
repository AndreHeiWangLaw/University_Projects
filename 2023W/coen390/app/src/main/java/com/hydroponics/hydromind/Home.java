package com.hydroponics.hydromind;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
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

import java.util.Locale;
import java.util.Objects;

public class Home extends AppCompatActivity {

    private TextView welcomeTextView;
    private ProgressBar progressBar;
    private String fullName;
    private TextView tdsValueText, phValueText, AirHumidityText, AirTemperatureText;
    private float tdsValue, phValue, AirHumidity, AirTemperature;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);
        Objects.requireNonNull(getSupportActionBar()).hide();

        welcomeTextView = findViewById(R.id.welcomeText);
        progressBar = findViewById(R.id.progressBar);
        CardView dashboardButton = findViewById(R.id.dashboard);
        CardView controlPanelButton = findViewById(R.id.status);
        CardView settingsButton = findViewById(R.id.settings);
        tdsValueText = findViewById(R.id.tdsSensorValue);
        phValueText = findViewById(R.id.phSensorValue);
        AirHumidityText = findViewById(R.id.HumidityValue);
        AirTemperatureText = findViewById(R.id.airTempValue);

        FirebaseAuth authProfile = FirebaseAuth.getInstance();
        FirebaseUser firebaseUser = authProfile.getCurrentUser();

        if (firebaseUser == null){
            Toast.makeText(this, "Something went Wrong! User details are not available at the moment! Please try again later.", Toast.LENGTH_SHORT).show();
        } else {
            progressBar.setVisibility(View.VISIBLE);
            showUserProfile(firebaseUser);
        }

        DatabaseReference databaseReference = FirebaseDatabase.getInstance().getReference();

        if (firebaseUser == null) {
            Toast.makeText(this, "Something went wrong! Please try again later.", Toast.LENGTH_SHORT).show();
        } else {
            showTdsValue(firebaseUser);
            showPhValue(firebaseUser);
            showHumidityValue(firebaseUser);
            showAirTempValue(firebaseUser);
        }

        dashboardButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openDashboard();
            }
        });

        controlPanelButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openControlPanel();
            }
        });

        settingsButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openSettings();
            }
        });

    }
    private void showUserProfile(FirebaseUser firebaseUser) {
        String userID = firebaseUser.getUid();

        DatabaseReference referenceProfile = FirebaseDatabase.getInstance().getReference("Registered Users");
        referenceProfile.child(userID).addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                fullName = firebaseUser.getDisplayName();

                welcomeTextView.setText("Welcome, " + fullName + "!");
                progressBar.setVisibility(View.GONE);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(Home.this, "Signed Out!", Toast.LENGTH_SHORT).show();
                progressBar.setVisibility(View.GONE);
            }
        });
    }
    private void openDashboard(){
        Intent intent = new Intent(this,Dashboard.class);
        startActivity(intent);
    }
    private void openControlPanel(){
        Intent intent = new Intent(this, ControlPanel.class);
        startActivity(intent);
    }
    private void openSettings(){
        Intent intent = new Intent(this,Settings.class);
        startActivity(intent);
    }

    private void showTdsValue(FirebaseUser firebaseUser) {
        String userID = firebaseUser.getUid();
        DatabaseReference tdsSensorValueRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("tdsSensorValue");
        tdsSensorValueRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    tdsValue = snapshot.getValue(Float.class);
                    tdsValueText.setText(String.format(Locale.getDefault(), "%.2f", tdsValue));
                } else {
                    showAlertDialog();
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(Home.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void showPhValue(FirebaseUser firebaseUser) {
        String userID = firebaseUser.getUid();
        DatabaseReference phSensorValueRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("pH_SensorValue");
        phSensorValueRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    phValue = snapshot.getValue(Float.class);
                    phValueText.setText(String.format(Locale.getDefault(), "%.2f", phValue));
                } else {
                    showAlertDialog();
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(Home.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void showAirTempValue(FirebaseUser firebaseUser) {
        String userID = firebaseUser.getUid();
        DatabaseReference airTempSensorValueRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("AirTemperature");
        airTempSensorValueRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    AirTemperature = snapshot.getValue(Float.class);
                    AirTemperatureText.setText(String.format(Locale.getDefault(), "%.2f", AirTemperature) + "°C");
                } else {
                    showAlertDialog();
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(Home.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
    }


    private void showHumidityValue(FirebaseUser firebaseUser) {
        String userID = firebaseUser.getUid();
        DatabaseReference airHumiditySensorValueRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("AirHumidity");
        airHumiditySensorValueRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    AirHumidity = snapshot.getValue(Float.class);
                    AirHumidityText.setText(String.format(Locale.getDefault(), "%.2f", AirHumidity) + "%");
                } else {
                    showAlertDialog();
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(Home.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void showAlertDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(Home.this);
        builder.setTitle("ESP32 Not Connected");
        builder.setMessage("Please verify your ESP32 micro-controller is connected to the wifi and to your account.");
        builder.setPositiveButton("Continue", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                openSettings();
            }
        });
        AlertDialog alertDialog = builder.create();
        alertDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialog) {
                openSettings();
            }
        });
        alertDialog.show();
    }
}