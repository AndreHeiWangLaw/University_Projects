package com.hydroponics.hydromind;

import static android.content.ContentValues.TAG;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.formatter.IndexAxisValueFormatter;
import com.github.mikephil.charting.formatter.ValueFormatter;
import com.github.mikephil.charting.highlight.Highlight;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.listener.ChartTouchListener;
import com.github.mikephil.charting.listener.OnChartGestureListener;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;
import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;

import java.util.Date;
import java.util.List;
import java.util.Locale;


public class Dashboard extends AppCompatActivity {
    private TextView tdsValueText, phValueText, selectedValue, selectedTimestamp, selectedValue2, selectedTimestamp2, AirHumidityText, AirTemperatureText;
    private float tdsValue, phValue, AirHumidity, AirTemperature;
    private DatabaseReference databaseReference;
    private FirebaseAuth authProfile;
    private BottomNavigationView bottomNavigationView;
    private LineChart lineChart, lineChart2;
    private int toggleState = 0, toggleState2 = 0; // 0: Both, 1: TDS only, 2: pH only

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dashboard);

        getSupportActionBar().hide();

        tdsValueText = findViewById(R.id.tdsSensorValue);
        phValueText = findViewById(R.id.ph_sensor_value);
        selectedValue = findViewById(R.id.graphValue1);
        selectedTimestamp = findViewById(R.id.timestamp1);
        selectedValue2 = findViewById(R.id.graphValue2);
        selectedTimestamp2 = findViewById(R.id.timestamp2);
        AirHumidityText = findViewById(R.id.humidity_value);
        AirTemperatureText = findViewById(R.id.airTemp_value);

        bottomNavigationView = findViewById(R.id.bottom_navigation);
        bottomNavigationView.setOnNavigationItemSelectedListener(navListener);
        bottomNavigationView.setSelectedItemId(R.id.nav_dashboard);

        Button toggleButton = findViewById(R.id.toggleButton1);
        Button toggleButton2 = findViewById(R.id.toggleButton2);
        lineChart = findViewById(R.id.tds_pH_chart);
        lineChart2 = findViewById(R.id.temp_hum_chart);

        lineChart.setOnChartGestureListener(new OnChartGestureListener() {
            @Override
            public void onChartGestureStart(MotionEvent me, ChartTouchListener.ChartGesture lastPerformedGesture) {
                //openGraphView();
            }

            @Override
            public void onChartGestureEnd(MotionEvent me, ChartTouchListener.ChartGesture lastPerformedGesture) {
                //openGraphView();
            }

            @Override
            public void onChartLongPressed(MotionEvent me) {
            }

            @Override
            public void onChartDoubleTapped(MotionEvent me) {
                openGraphView();
            }

            @Override
            public void onChartSingleTapped(MotionEvent me) {
                //openGraphView();
            }

            @Override
            public void onChartFling(MotionEvent me1, MotionEvent me2, float velocityX, float velocityY) {
            }

            @Override
            public void onChartScale(MotionEvent me, float scaleX, float scaleY) {
            }

            @Override
            public void onChartTranslate(MotionEvent me, float dX, float dY) {
            }
        });

        lineChart2.setOnChartGestureListener(new OnChartGestureListener() {
            @Override
            public void onChartGestureStart(MotionEvent me, ChartTouchListener.ChartGesture lastPerformedGesture) {
                //openGraphView();
            }

            @Override
            public void onChartGestureEnd(MotionEvent me, ChartTouchListener.ChartGesture lastPerformedGesture) {
                //openGraphView();
            }

            @Override
            public void onChartLongPressed(MotionEvent me) {
            }

            @Override
            public void onChartDoubleTapped(MotionEvent me) {
                openGraph2View();
            }

            @Override
            public void onChartSingleTapped(MotionEvent me) {
                //openGraphView();
            }

            @Override
            public void onChartFling(MotionEvent me1, MotionEvent me2, float velocityX, float velocityY) {
            }

            @Override
            public void onChartScale(MotionEvent me, float scaleX, float scaleY) {
            }

            @Override
            public void onChartTranslate(MotionEvent me, float dX, float dY) {
            }
        });


        authProfile = FirebaseAuth.getInstance();
        databaseReference = FirebaseDatabase.getInstance().getReference();

        FirebaseUser firebaseUser = authProfile.getCurrentUser();
        String userId = firebaseUser.getUid();

        if (firebaseUser == null) {
            Toast.makeText(this, "Something went wrong! Please try again later.", Toast.LENGTH_SHORT).show();
        } else {
            showTdsValue(firebaseUser);
            showPhValue(firebaseUser);
            showHumidityValue(firebaseUser);
            showAirTempValue(firebaseUser);
        }

        DatabaseReference sensorHistoryRef = databaseReference.child("Registered Users").child(userId).child("SensorsHistory");
        sensorHistoryRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                ArrayList<Entry> tdsEntries = new ArrayList<>();
                ArrayList<Entry> phEntries = new ArrayList<>();
                ArrayList<String> labels = new ArrayList<>();
                SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
                int i = 0;
                List<DataSnapshot> snapshots = new ArrayList<>();
                for (DataSnapshot sampleSnapshot : dataSnapshot.getChildren()) {
                    snapshots.add(sampleSnapshot);
                    if (snapshots.size() > 5) {
                        snapshots.remove(0);
                    }
                }
                for (DataSnapshot sampleSnapshot : snapshots) {
                    for (DataSnapshot data : sampleSnapshot.getChildren()) {
                        if (data.getKey().equals("time")) {
                            String time = data.getValue(String.class);
                            if (time != null) {
                                try {
                                    Date date = new SimpleDateFormat("HH:mm", Locale.getDefault()).parse(time);
                                    tdsEntries.add(new Entry(i, sampleSnapshot.child("tdsValue").getValue(Float.class)));
                                    phEntries.add(new Entry(i, sampleSnapshot.child("phValue").getValue(Float.class)));
                                    String label = timeFormat.format(date);
                                    if ("00:00".equals(label)) {
                                        label = dateFormat.format(date);
                                    }
                                    labels.add(label);
                                    i++;
                                } catch (ParseException e) {
                                    e.printStackTrace();
                                }
                            }
                        }
                    }
                }

                // Create a line data set for TDS values and add it to the line chart
                LineDataSet tdsDataSet = new LineDataSet(tdsEntries, "TDS Values");
                tdsDataSet.setColor(0xFF004D40);
                tdsDataSet.setCircleColor(0xFF004D40);

                // Create a line data set for pH values and add it to the line chart
                LineDataSet phDataSet = new LineDataSet(phEntries, "pH Values");
                phDataSet.setColor(0xFF02BBA9);
                phDataSet.setCircleColor(0xFF02BBA9);
                phDataSet.setAxisDependency(YAxis.AxisDependency.RIGHT); // set y axis to right for pH values
                lineChart.setOnChartValueSelectedListener(new OnChartValueSelectedListener() {
                    @Override
                    public void onValueSelected(Entry entry, Highlight highlight) {
                        // Get the date value from the selected sample snapshot
                        DataSnapshot selectedSampleSnapshot = snapshots.get((int) entry.getX());
                        String dateLabel = selectedSampleSnapshot.child("date").getValue(String.class);

                        // Display the value of the selected sample in a Toast
                        selectedValue.setText(String.valueOf(entry.getY()));
                        String timestampLabel = labels.get((int) entry.getX());
                        selectedTimestamp.setText(timestampLabel + " on " + dateLabel);
                    }

                    @Override
                    public void onNothingSelected() {
                        // Hide the value of the selected sample
                    }
                });

                // Add both line data sets to a list and create the line data object
                List<ILineDataSet> dataSets = new ArrayList<>();
                dataSets.add(tdsDataSet);
                dataSets.add(phDataSet);
                LineData lineData = new LineData(dataSets);


                // Set the line data object to the line chart
                lineChart.setData(lineData);

                // Set the X-axis values
                XAxis xAxis = lineChart.getXAxis();
                xAxis.setValueFormatter(new IndexAxisValueFormatter(labels));
                xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
                xAxis.setTextSize(10f); // Increase font size of labels
                lineChart.getDescription().setText("Last 5 TDS and pH samples");
                lineChart.getXAxis().setLabelCount(labels.size());
                lineChart.getLegend().setEnabled(true);

                tdsDataSet.setValueFormatter(new ValueFormatter() {
                    private DecimalFormat decimalFormat = new DecimalFormat("#.00");
                    @Override
                    public String getFormattedValue(float value) {
                        return decimalFormat.format(value);
                    }
                });

                phDataSet.setValueFormatter(new ValueFormatter() {
                    private DecimalFormat decimalFormat = new DecimalFormat("#.00");
                    @Override
                    public String getFormattedValue(float value) {
                        return decimalFormat.format(value);
                    }
                });

                // Set the Y-axis value formatter for the pH values
                YAxis rightAxis = lineChart.getAxisRight();
                rightAxis.setEnabled(true); // Enable drawing of the right y-axis labels
                rightAxis.setPosition(YAxis.YAxisLabelPosition.OUTSIDE_CHART);
                rightAxis.setTextSize(10f); // Increase font size of labels

                // Set the Y-axis value formatter for the TDS values
                YAxis leftAxis = lineChart.getAxisLeft();
                leftAxis.setTextSize(10f); // Increase font size of labels

                // Invalidate the line chart to refresh it
                lineChart.invalidate();

                toggleButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        toggleState = (toggleState + 1) % 3;
                        if (lineChart.getData() != null) {
                            List<ILineDataSet> dataSets = lineChart.getData().getDataSets();
                            if (toggleState == 0) { // Both TDS and pH plots
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(true);
                                }
                                lineChart.getAxisLeft().setEnabled(true);
                                lineChart.getAxisRight().setEnabled(true);
                                lineChart.getDescription().setText("Last 5 TDS and pH samples");
                            } else if (toggleState == 1) { // TDS plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("TDS Values"));
                                }
                                lineChart.getAxisLeft().setEnabled(true);
                                lineChart.getAxisRight().setEnabled(false);
                                lineChart.getDescription().setText("Last 5 TDS samples");
                            } else { // pH plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("pH Values"));
                                }
                                lineChart.getAxisLeft().setEnabled(false);
                                lineChart.getAxisRight().setEnabled(true);
                                lineChart.getDescription().setText("Last 5 pH samples");
                            }
                            lineChart.invalidate(); // Refresh the chart
                        }
                    }
                });
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w(TAG, "Failed to read values.", databaseError.toException());
            }
        });

        sensorHistoryRef = databaseReference.child("Registered Users").child(userId).child("SensorsHistory");
        sensorHistoryRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                ArrayList<Entry> tempEntries = new ArrayList<>();
                ArrayList<Entry> humEntries = new ArrayList<>();
                ArrayList<String> labels = new ArrayList<>();
                SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());
                SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
                int i = 0;
                List<DataSnapshot> snapshots = new ArrayList<>();
                for (DataSnapshot sampleSnapshot : dataSnapshot.getChildren()) {
                    snapshots.add(sampleSnapshot);
                    if (snapshots.size() > 5) {
                        snapshots.remove(0);
                    }
                }
                for (DataSnapshot sampleSnapshot : snapshots) {
                    for (DataSnapshot data : sampleSnapshot.getChildren()) {
                        if (data.getKey().equals("time")) {
                            String time = data.getValue(String.class);
                            if (time != null) {
                                try {
                                    Date date = new SimpleDateFormat("HH:mm", Locale.getDefault()).parse(time);
                                    tempEntries.add(new Entry(i, sampleSnapshot.child("AirTemperature").getValue(Float.class)));
                                    humEntries.add(new Entry(i, sampleSnapshot.child("AirHumidity").getValue(Float.class)));
                                    String label = timeFormat.format(date);
                                    labels.add(label);
                                    i++;
                                } catch (ParseException e) {
                                    e.printStackTrace();
                                }
                            }
                        }
                    }
                }

                // Create a line data set for TDS values and add it to the line chart
                LineDataSet tempDataSet = new LineDataSet(tempEntries, "Temperature Values");
                tempDataSet.setColor(0xFF012147);
                tempDataSet.setCircleColor(0xFF012147);

                // Create a line data set for pH values and add it to the line chart
                LineDataSet HumidityDataSet = new LineDataSet(humEntries, "Humidity Values");
                HumidityDataSet.setColor(0xFF00d6d0);
                HumidityDataSet.setCircleColor(0xFF00d6d0);
                HumidityDataSet.setAxisDependency(YAxis.AxisDependency.RIGHT); // set y axis to right for pH values
                lineChart2.setOnChartValueSelectedListener(new OnChartValueSelectedListener() {
                    @Override
                    public void onValueSelected(Entry entry, Highlight highlight) {
                        // Get the date value from the selected sample snapshot
                        DataSnapshot selectedSampleSnapshot = snapshots.get((int) entry.getX());
                        String dateLabel = selectedSampleSnapshot.child("date").getValue(String.class);

                        // Display the value of the selected sample in a Toast
                        selectedValue2.setText(String.valueOf(entry.getY()));
                        String timestampLabel = labels.get((int) entry.getX());
                        selectedTimestamp2.setText(timestampLabel + " on " + dateLabel);
                    }


                    @Override
                    public void onNothingSelected() {
                        // Hide the value of the selected sample
                    }
                });

                // Add both line data sets to a list and create the line data object
                List<ILineDataSet> dataSets = new ArrayList<>();
                dataSets.add(tempDataSet);
                dataSets.add(HumidityDataSet);
                LineData lineData = new LineData(dataSets);


                // Set the line data object to the line chart
                lineChart2.setData(lineData);

                // Set the X-axis values
                XAxis xAxis = lineChart2.getXAxis();
                xAxis.setValueFormatter(new IndexAxisValueFormatter(labels));
                xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
                xAxis.setTextSize(10f); // Increase font size of labels
                lineChart2.getDescription().setText("Last 5 Temperature and Humidity samples");
                lineChart2.getXAxis().setLabelCount(labels.size());
                lineChart2.getLegend().setEnabled(true);

                tempDataSet.setValueFormatter(new ValueFormatter() {
                    private DecimalFormat decimalFormat = new DecimalFormat("#.00");
                    @Override
                    public String getFormattedValue(float value) {
                        return decimalFormat.format(value);
                    }
                });

                HumidityDataSet.setValueFormatter(new ValueFormatter() {
                    private DecimalFormat decimalFormat = new DecimalFormat("#.00");
                    @Override
                    public String getFormattedValue(float value) {
                        return decimalFormat.format(value);
                    }
                });

                // Set the Y-axis value formatter for the pH values
                YAxis rightAxis = lineChart2.getAxisRight();
                rightAxis.setEnabled(true); // Enable drawing of the right y-axis labels
                rightAxis.setPosition(YAxis.YAxisLabelPosition.OUTSIDE_CHART);
                rightAxis.setTextSize(10f); // Increase font size of labels

                // Set the Y-axis value formatter for the TDS values
                YAxis leftAxis = lineChart2.getAxisLeft();
                leftAxis.setTextSize(10f); // Increase font size of labels

                // Invalidate the line chart to refresh it
                lineChart2.invalidate();

                toggleButton2.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        toggleState2 = (toggleState2 + 1) % 3;
                        if (lineChart2.getData() != null) {
                            List<ILineDataSet> dataSets = lineChart2.getData().getDataSets();
                            if (toggleState2 == 0) { // Both TDS and pH plots
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(true);
                                }
                                lineChart2.getAxisLeft().setEnabled(true);
                                lineChart2.getAxisRight().setEnabled(true);
                                lineChart2.getDescription().setText("Last 5 Temperature and Humidity samples");
                            } else if (toggleState2 == 1) { // TDS plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("Temperature Values"));
                                }
                                lineChart2.getAxisLeft().setEnabled(true);
                                lineChart2.getAxisRight().setEnabled(false);
                                lineChart2.getDescription().setText("Last 5 Temperature samples");
                            } else { // pH plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("Humidity Values"));
                                }
                                lineChart2.getAxisLeft().setEnabled(false);
                                lineChart2.getAxisRight().setEnabled(true);
                                lineChart2.getDescription().setText("Last 5 Humidity samples");
                            }
                            lineChart2.invalidate(); // Refresh the chart
                        }
                    }
                });
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w(TAG, "Failed to read values.", databaseError.toException());
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
                            break;
                        case R.id.nav_cp:
                            openControlPanel();
                            break;
                        default:
                            return false;
                    }
                    return true;
                }
            };

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
                Toast.makeText(Dashboard.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
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
                Toast.makeText(Dashboard.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
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
                Toast.makeText(Dashboard.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
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
                    AirHumidityText.setText(String.format(Locale.getDefault(), "%.2f", AirHumidity) + " %");
                } else {
                    showAlertDialog();
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Toast.makeText(Dashboard.this, "Something went wrong!", Toast.LENGTH_SHORT).show();
            }
        });
    }


    private void openControlPanel() {
        Intent intent = new Intent(this, ControlPanel.class);
        startActivity(intent);
    }

    private void openHome() {
        Intent intent = new Intent(this, Home.class);
        startActivity(intent);
    }

    private void openGraphView() {
        Intent intent = new Intent(this, GraphViewActivity.class);
        startActivity(intent);
    }

    private void openGraph2View() {
        Intent intent = new Intent(this, GraphViewActivity2.class);
        startActivity(intent);
    }

    private void openUserProfile() {
        Intent intent = new Intent(this, Home.class);
        startActivity(intent);
    }

    private void showAlertDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(Dashboard.this);
        builder.setTitle("ESP32 Not Connected");
        builder.setMessage("Please verify your ESP32 micro-controller is connected to the wifi and to your account.");
        builder.setPositiveButton("Continue", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                openUserProfile();
            }
        });
        AlertDialog alertDialog = builder.create();
        alertDialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialog) {
                openUserProfile();
            }
        });
        alertDialog.show();
    }
}