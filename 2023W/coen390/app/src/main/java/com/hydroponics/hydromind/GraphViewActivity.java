package com.hydroponics.hydromind;

import static android.content.ContentValues.TAG;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IndexAxisValueFormatter;
import com.github.mikephil.charting.formatter.ValueFormatter;
import com.github.mikephil.charting.highlight.Highlight;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Locale;

public class GraphViewActivity extends AppCompatActivity {

    private DatabaseReference databaseReference, sensorHistoryRef;
    private FirebaseAuth authProfile;
    private TextView selectedValue, selectedTimestamp;
    private LineChart lineChart;
    private int numberOfSamples = 10; // Default number of samples
    private int toggleState = 0; // 0: Both, 1: TDS only, 2: pH only

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_graph_view);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle("TDS and pH");

        lineChart = findViewById(R.id.tds_pH_chart);
        selectedValue = findViewById(R.id.graphValue1);
        selectedTimestamp = findViewById(R.id.timestamp1);

        authProfile = FirebaseAuth.getInstance();
        databaseReference = FirebaseDatabase.getInstance().getReference();

        FirebaseUser firebaseUser = authProfile.getCurrentUser();
        String userId = firebaseUser.getUid();

        sensorHistoryRef = databaseReference.child("Registered Users").child(userId).child("SensorsHistory");
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
                    if (snapshots.size() > numberOfSamples) {
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
                                    if (i % 5 == 0) { // only add label every 3 iterations
                                        String label = timeFormat.format(date);
                                        labels.add(label);
                                    } else {
                                        labels.add("");
                                    }
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
                        String timestampLabel = selectedSampleSnapshot.child("time").getValue(String.class);
                        // Display the value of the selected sample in a Toast
                        selectedTimestamp.setText(timestampLabel + " on " + dateLabel);
                        selectedValue.setText(String.valueOf(entry.getY()));
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
                lineChart.getDescription().setText("Last 10 TDS and pH samples");
                lineChart.getXAxis().setLabelCount(labels.size());
                lineChart.getLegend().setEnabled(true);
                Legend legend = lineChart.getLegend();
                legend.setVerticalAlignment(Legend.LegendVerticalAlignment.TOP);

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

                Button toggleButton = findViewById(R.id.toggleButton1);
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
                                lineChart.getDescription().setText("Last " + numberOfSamples + " TDS and pH samples");
                            } else if (toggleState == 1) { // TDS plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("TDS Values"));
                                }
                                lineChart.getAxisLeft().setEnabled(true);
                                lineChart.getAxisRight().setEnabled(false);
                                lineChart.getDescription().setText("Last " + numberOfSamples + " TDS samples");
                            } else { // pH plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("pH Values"));
                                }
                                lineChart.getAxisLeft().setEnabled(false);
                                lineChart.getAxisRight().setEnabled(true);
                                lineChart.getDescription().setText("Last " + numberOfSamples + " pH samples");
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
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.graph_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
            case R.id.action_sample_10:
                updateNumberOfSamples(10);
                return true;
            case R.id.action_sample_30:
                updateNumberOfSamples(30);
                return true;
            case R.id.action_sample_50:
                updateNumberOfSamples(50);
                return true;
            case R.id.action_historical_view:
                openHistoricalViewActivity();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }


    private void updateNumberOfSamples(int newNumberOfSamples) {
        numberOfSamples = newNumberOfSamples;
        sensorHistoryRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                ArrayList<Entry> tdsEntries = new ArrayList<>();
                ArrayList<Entry> phEntries = new ArrayList<>();
                ArrayList<String> labels = new ArrayList<>();
                SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());
                int i = 0;
                List<DataSnapshot> snapshots = new ArrayList<>();
                for (DataSnapshot sampleSnapshot : dataSnapshot.getChildren()) {
                    snapshots.add(sampleSnapshot);
                    if (snapshots.size() > numberOfSamples) {
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
                                    if (i % 5 == 0) { // only add label every 3 iterations
                                        String label = timeFormat.format(date);
                                        labels.add(label);
                                    } else {
                                        labels.add("");
                                    }
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
                tdsDataSet.setDrawValues(false);

                // Create a line data set for pH values and add it to the line chart
                LineDataSet phDataSet = new LineDataSet(phEntries, "pH Values");
                phDataSet.setColor(0xFF02BBA9);
                phDataSet.setCircleColor(0xFF02BBA9);
                phDataSet.setDrawValues(false);
                phDataSet.setAxisDependency(YAxis.AxisDependency.RIGHT); // set y axis to right for pH values

                lineChart.setOnChartValueSelectedListener(new OnChartValueSelectedListener() {
                    @Override
                    public void onValueSelected(Entry entry, Highlight highlight) {
                        // Get the date value from the selected sample snapshot
                        DataSnapshot selectedSampleSnapshot = snapshots.get((int) entry.getX());
                        String dateLabel = selectedSampleSnapshot.child("date").getValue(String.class);
                        String timestampLabel = selectedSampleSnapshot.child("time").getValue(String.class);
                        selectedTimestamp.setText(timestampLabel + " on " + dateLabel);
                        selectedValue.setText(String.valueOf(entry.getY()));
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
                lineChart.getDescription().setText("Last " + newNumberOfSamples + " TDS and pH samples");
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

                Button toggleButton = findViewById(R.id.toggleButton1);
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
                                lineChart.getDescription().setText("Last " + newNumberOfSamples + " TDS and pH samples");
                            } else if (toggleState == 1) { // TDS plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("TDS Values"));
                                }
                                lineChart.getAxisLeft().setEnabled(true);
                                lineChart.getAxisRight().setEnabled(false);
                                lineChart.getDescription().setText("Last " + newNumberOfSamples + " TDS samples");
                            } else { // pH plot only
                                for (ILineDataSet dataSet : dataSets) {
                                    dataSet.setVisible(dataSet.getLabel().equals("pH Values"));
                                }
                                lineChart.getAxisLeft().setEnabled(false);
                                lineChart.getAxisRight().setEnabled(true);
                                lineChart.getDescription().setText("Last " + newNumberOfSamples + " pH samples");
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
    }
    private void openHistoricalViewActivity() {
        Intent intent = new Intent(GraphViewActivity.this, HistoricalViewActivity.class);
        startActivity(intent);
    }


}