package com.hydroponics.hydromind;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

public class ControlPanel extends AppCompatActivity {

    private EditText tdsMin, tdsMax, phMin, phMax;
    private String tdsMinText, tdsMaxText, phMinText, phMaxText;
    private Double tdsMinDouble, tdsMaxDouble, phMinDouble, phMaxDouble;
    private TextView tdsValueText, phValueText;
    private Float tdsValue, phValue;
    private BottomNavigationView bottomNavigationView;
    private ToggleButton pumpA ,pumpB, pumpC, pumpD, tds, ph;
    private LineChart lineChart1;
    private Switch tdsAuto, phAuto;
    private Timer timer;
    private ArrayList<Entry> tdsData = new ArrayList<>();
    private ArrayList<Entry> phData = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control_panel);
        getSupportActionBar().hide();

        tdsMin = findViewById(R.id.minTdsLimit);
        tdsMax = findViewById(R.id.maxTdsLimit);
        phMin = findViewById(R.id.minPhLimit);
        phMax = findViewById(R.id.maxPhLimit);
        pumpA = findViewById(R.id.up_ph);
        pumpB = findViewById(R.id.down_ph);
        pumpC = findViewById(R.id.down_tds);
        pumpD = findViewById(R.id.up_tds);
        tds = findViewById(R.id.tdsToggle);
        ph = findViewById(R.id.phToggle);
        tdsValueText = findViewById(R.id.txt_tds_sensor_value);
        phValueText = findViewById(R.id.txt_ph_sensor_value);
        lineChart1 = findViewById(R.id.tds_pH_chart);
        tdsAuto = findViewById(R.id.btn_tds_auto);
        phAuto = findViewById(R.id.btn_ph_auto);

        bottomNavigationView = findViewById(R.id.bottom_navigation);
        bottomNavigationView.setOnNavigationItemSelectedListener(navListener);
        bottomNavigationView.setSelectedItemId(R.id.nav_cp);

        Button editButton = findViewById(R.id.editButton);
        Button saveButton = findViewById(R.id.saveButton);

        startDataUpdate();

        editButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tdsMin.setEnabled(true);
                tdsMax.setEnabled(true);
                phMin.setEnabled(true);
                phMax.setEnabled(true);
                saveButton.setVisibility(View.VISIBLE);
            }
        });

        FirebaseAuth auth = FirebaseAuth.getInstance();
        FirebaseUser firebaseUser = auth.getCurrentUser();
        String userID = firebaseUser.getUid();

        if (firebaseUser == null) {
            Toast.makeText(this, "Something went wrong! Please try again later.", Toast.LENGTH_SHORT).show();
        } else {
            showTdsValue(firebaseUser);
            showPhValue(firebaseUser);
        }

        DatabaseReference tdsMinRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("tdsMin");
        DatabaseReference tdsMaxRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("tdsMax");
        DatabaseReference phMinRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("phMin");
        DatabaseReference phMaxRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("phMax");

        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                tdsMinText = tdsMin.getText().toString();
                tdsMaxText = tdsMax.getText().toString();
                phMinText = phMin.getText().toString();
                phMaxText = phMax.getText().toString();

                tdsMinDouble = tdsMinText.isEmpty() ? null : Double.parseDouble(tdsMinText);
                tdsMaxDouble = tdsMaxText.isEmpty() ? null : Double.parseDouble(tdsMaxText);
                phMinDouble = phMinText.isEmpty() ? null : Double.parseDouble(phMinText);
                phMaxDouble = phMaxText.isEmpty() ? null : Double.parseDouble(phMaxText);

                tdsMin.setEnabled(false);
                tdsMax.setEnabled(false);
                phMin.setEnabled(false);
                phMax.setEnabled(false);

                saveButton.setVisibility(View.GONE);
                showLimits(firebaseUser);

                if ((tdsMinDouble != null && tdsMaxDouble != null && tdsMinDouble >= tdsMaxDouble) ||
                        (phMinDouble != null && phMaxDouble != null && phMinDouble >= phMaxDouble)) {
                    Toast.makeText(ControlPanel.this, "Minimum value must be smaller than maximum value!", Toast.LENGTH_SHORT).show();
                    tdsMin.setText("");
                    tdsMax.setText("");
                    phMin.setText("");
                    phMax.setText("");
                } else if ((tdsMinDouble != null && (tdsMinDouble < 100 || tdsMinDouble > 2000)) ||
                        (tdsMaxDouble != null && (tdsMaxDouble < 100 || tdsMaxDouble > 2000)) ||
                        (phMinDouble != null && (phMinDouble < 1 || phMinDouble > 14)) ||
                        (phMaxDouble != null && (phMaxDouble < 1 || phMaxDouble > 14))) {
                    tdsMin.setText("");
                    tdsMax.setText("");
                    phMin.setText("");
                    phMax.setText("");
                    Toast.makeText(ControlPanel.this, "Make sure the values entered are within the ranges!\n TDS: 100-2000 ppm | ph : 1-14", Toast.LENGTH_SHORT).show();
                } else {
                    if (tdsMinDouble != null) {
                        float tdsMinFloat = Float.parseFloat(tdsMinText);
                        tdsMinRef.setValue(tdsMinFloat);
                    }
                    if (tdsMaxDouble != null) {
                        float tdsMaxFloat = Float.parseFloat(tdsMaxText);
                        tdsMaxRef.setValue(tdsMaxFloat);
                    }
                    if (phMinDouble != null) {
                        float phMinFloat = Float.parseFloat(phMinText);
                        phMinRef.setValue(phMinFloat);
                    }
                    if (phMaxDouble != null) {
                        float phMaxFloat = Float.parseFloat(phMaxText);
                        phMaxRef.setValue(phMaxFloat);
                    }
                }
            }
        });
        tds.setChecked(true);
        tds.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    tds.setTextColor(0xFF005EFF);
                    ph.setChecked(false);
                    setTdsPlotVisibility(true); // Show TDS plot
                    setPhPlotVisibility(false);

                }else {
                    setTdsPlotVisibility(false); // Show TDS plot
                    tds.setTextColor(0xFF000000);
                }
            }
        });
        ph.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    ph.setTextColor(0xFF005EFF);
                    tds.setChecked(false);
                    setPhPlotVisibility(true);
                    setTdsPlotVisibility(false);
                } else {
                    setPhPlotVisibility(false);
                    ph.setTextColor(0xFF000000);
                }
            }
        });

        DatabaseReference pumpARef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("pumpA");
        pumpA.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // Handle the switch state change here
                if (isChecked) {
                    pumpARef.setValue(true);
                    pumpA.setBackgroundResource(R.drawable.uparrowbold_on);
                } else {
                    pumpARef.setValue(false);
                    pumpA.setBackgroundResource(R.drawable.uparrowbold);
                }
            }
        });

        DatabaseReference pumpBRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("pumpB");
        pumpB.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                //Handle the switch state change here
                if (isChecked) {
                    Toast.makeText(ControlPanel.this, "Pumping down pH", Toast.LENGTH_SHORT).show();
                    pumpBRef.setValue(true);
                    pumpB.setBackgroundResource(R.drawable.downarrowbold_on);
                } else {
                    pumpBRef.setValue(false);
                    pumpB.setBackgroundResource(R.drawable.downarrowbold);
                }
            }
        });

        DatabaseReference pumpCRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("pumpC");
        pumpC.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // Handle the switch state change here
                if (isChecked) {
                    pumpCRef.setValue(true);
                    pumpC.setBackgroundResource(R.drawable.downarrowbold_on);
                } else {
                    pumpCRef.setValue(false);
                    pumpC.setBackgroundResource(R.drawable.downarrowbold);
                }
            }
        });

        DatabaseReference pumpDRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("pumpD");
        pumpD.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                // Handle the switch state change here
                if (isChecked) {
                    pumpDRef.setValue(true);
                    pumpD.setBackgroundResource(R.drawable.uparrowbold_on);
                } else {
                    pumpDRef.setValue(false);
                    pumpD.setBackgroundResource(R.drawable.uparrowbold);
                }
            }
        });

        DatabaseReference tdsAutoRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("tdsAuto");
        tdsAuto.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked){
                    tdsAutoRef.setValue(true);
                } else {
                    tdsAutoRef.setValue(false);
                }
            }
        });

        DatabaseReference phAutoRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("phAuto");
        phAuto.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked){
                    phAutoRef.setValue(true);
                } else {
                    phAutoRef.setValue(false);
                }
            }
        });
    }

    private void setPhPlotVisibility(boolean visible) {
        LineData lineData = lineChart1.getData();
        if (lineData != null) {
            LineDataSet phDataSet = (LineDataSet) lineData.getDataSetByLabel("pH", true);
            if (phDataSet != null) {
                phDataSet.setVisible(visible);
                lineChart1.invalidate(); // Refresh the chart
            }
        }
    }

    private void setTdsPlotVisibility(boolean visible) {
        LineData lineData = lineChart1.getData();
        if (lineData != null) {
            LineDataSet tdsDataSet = (LineDataSet) lineData.getDataSetByLabel("TDS", true);
            if (tdsDataSet != null) {
                tdsDataSet.setVisible(visible);
                lineChart1.invalidate(); // Refresh the chart
            }
        }
    }

    private void startDataUpdate() {
        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                updateChartData();
            }
        }, 0, 3000); // Update every 3 seconds
    }

    private void updateChartData() {

        FirebaseAuth auth = FirebaseAuth.getInstance();
        FirebaseUser firebaseUser = auth.getCurrentUser();
        String userID = firebaseUser.getUid();

        DatabaseReference tdsSensorValueRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("tdsSensorValue");
        DatabaseReference phSensorValueRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("pH_SensorValue");
        ValueEventListener tdsSensorListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                float tdsValue = dataSnapshot.getValue(Float.class);
                tdsData.add(new Entry(tdsData.size(), tdsValue));
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        };

        ValueEventListener phSensorListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                float phValue = dataSnapshot.getValue(Float.class);
                phData.add(new Entry(phData.size(), phValue));

                updateLineChart();
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
            }
        };

        tdsSensorValueRef.addListenerForSingleValueEvent(tdsSensorListener);
        phSensorValueRef.addListenerForSingleValueEvent(phSensorListener);
    }

    private void updateLineChart() {
        LineDataSet tdsDataSet = new LineDataSet(tdsData, "TDS");
        LineDataSet phDataSet = new LineDataSet(phData, "pH");

        tdsDataSet.setAxisDependency(YAxis.AxisDependency.LEFT);
        phDataSet.setAxisDependency(YAxis.AxisDependency.RIGHT);

        tdsDataSet.setDrawCircles(false);
        phDataSet.setDrawCircles(false);

        tdsDataSet.setDrawValues(false);
        phDataSet.setDrawValues(false);

        boolean tdsVisible = tds.isChecked();
        boolean phVisible = ph.isChecked();
        tdsDataSet.setVisible(tdsVisible);
        phDataSet.setVisible(phVisible);

        YAxis leftYAxis = lineChart1.getAxisLeft();
        leftYAxis.setAxisMinimum(100);
        leftYAxis.setAxisMaximum(2000);
        YAxis rightYAxis = lineChart1.getAxisRight();
        rightYAxis.setAxisMinimum(1);
        rightYAxis.setAxisMaximum(14);
        leftYAxis.setEnabled(tdsVisible);
        rightYAxis.setEnabled(phVisible);

        // Customize the colors of the data sets (optional)
        tdsDataSet.setColor(Color.BLUE);
        phDataSet.setColor(Color.RED);
        tdsDataSet.setCircleColor(Color.BLUE);
        phDataSet.setCircleColor(Color.RED);

        LineData lineData = new LineData(tdsDataSet, phDataSet);
        lineChart1.setData(lineData);
        lineChart1.invalidate(); // Refresh the chart

        // Show the right Y-axis
        rightYAxis = lineChart1.getAxisRight();
        rightYAxis.setEnabled(true);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (timer != null) {
            timer.cancel();
        }
    }

    @Override
    protected void onStart() {
        super.onStart();

        FirebaseAuth auth = FirebaseAuth.getInstance();
        FirebaseUser firebaseUser = auth.getCurrentUser();

        if (firebaseUser == null){
            Toast.makeText(this, "Something went wrong! Try Again!", Toast.LENGTH_SHORT).show();
        } else {
            showLimits(firebaseUser);
        }

    }


    private void showLimits(FirebaseUser firebaseUser) {

        String userID = firebaseUser.getUid();

        DatabaseReference tdsMinRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("tdsMin");
        DatabaseReference tdsMaxRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("tdsMax");
        DatabaseReference phMinRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("phMin");
        DatabaseReference phMaxRef = FirebaseDatabase.getInstance().getReference("Registered Users").child(userID).child("phMax");

        tdsMinRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    float tdsMinFloat = snapshot.getValue(Float.class);
                    tdsMin.setHint(String.format(Locale.getDefault(), "%.2f", tdsMinFloat));
                } else {
                    Toast.makeText(ControlPanel.this, "Default values set: TDS - Min. : 300ppm, Max. : 1000ppm | pH - Min. : 5.5, Max. : 6.5", Toast.LENGTH_SHORT).show();
                    tdsMin.setHint("300");
                    tdsMax.setHint("700");
                    phMin.setHint("5.5");
                    phMax.setHint("6.5");

                    tdsMinRef.setValue(300);
                    tdsMaxRef.setValue(700);
                    phMinRef.setValue(5.5);
                    phMaxRef.setValue(6.5);
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(ControlPanel.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
        tdsMaxRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    float tdsMaxFloat = snapshot.getValue(Float.class);
                    tdsMax.setHint(String.format(Locale.getDefault(), "%.2f", tdsMaxFloat));
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(ControlPanel.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
        phMinRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    float phMinFloat = snapshot.getValue(Float.class);
                    phMin.setHint(String.format(Locale.getDefault(), "%.2f", phMinFloat));
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(ControlPanel.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
        phMaxRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (snapshot.exists()) {
                    float phMaxFloat = snapshot.getValue(Float.class);
                    phMax.setHint(String.format(Locale.getDefault(), "%.2f", phMaxFloat));
                }
            }
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(ControlPanel.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private BottomNavigationView.OnNavigationItemSelectedListener navListener =
            new BottomNavigationView.OnNavigationItemSelectedListener() {
                @Override
                public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                    switch (item.getItemId()) {
                        case R.id.nav_home:
                            openHome();
                            break;
                        case R.id.nav_dashboard:
                            openDashboard();
                            break;
                        case R.id.nav_cp:
                            break;
                        default:
                            return false;
                    }
                    return true;
                }
            };

    private void openDashboard() {
        Intent intent = new Intent(this, Dashboard.class);
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
                Toast.makeText(ControlPanel.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
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
                Toast.makeText(ControlPanel.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void showAlertDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(ControlPanel.this);
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

    private void openSettings(){
        Intent intent = new Intent(this, Settings.class);
        startActivity(intent);
    }

    private void openHome() {
        Intent intent = new Intent(this, Home.class);
        startActivity(intent);
    }
}
