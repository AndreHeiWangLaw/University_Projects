package com.hydroponics.hydromind;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.ListView;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

public class HistoricalViewActivity extends AppCompatActivity {

    private FirebaseAuth authProfile;
    private DatabaseReference databaseReference;
    private ListView sampleDataListView;

    // Add these lines for date and time formatting
    private SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd", Locale.getDefault());
    private SimpleDateFormat timeFormat = new SimpleDateFormat("HH:mm", Locale.getDefault());


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_historical_view);

        sampleDataListView = findViewById(R.id.historical_list_view);


        authProfile = FirebaseAuth.getInstance();
        databaseReference = FirebaseDatabase.getInstance().getReference();

        FirebaseUser firebaseUser = authProfile.getCurrentUser();
        String userId = firebaseUser.getUid();

        DatabaseReference sensorHistoryRef = databaseReference.child("Registered Users").child(userId).child("SensorsHistory");
        sensorHistoryRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                List<Map<String, String>> sampleDataList = new ArrayList<>();

                List<DataSnapshot> sortedSnapshots = new ArrayList<>();
                for (DataSnapshot sampleSnapshot : dataSnapshot.getChildren()) {
                    sortedSnapshots.add(sampleSnapshot);
                }

                // Sort the snapshots by date and time in ascending order
                Collections.sort(sortedSnapshots, new Comparator<DataSnapshot>() {
                    @Override
                    public int compare(DataSnapshot snapshot1, DataSnapshot snapshot2) {
                        String date1 = snapshot1.child("date").getValue(String.class);
                        String time1 = snapshot1.child("time").getValue(String.class);
                        String date2 = snapshot2.child("date").getValue(String.class);
                        String time2 = snapshot2.child("time").getValue(String.class);

                        try {
                            Date parsedDate1 = dateFormat.parse(date1);
                            Date parsedTime1 = timeFormat.parse(time1);
                            Date parsedDate2 = dateFormat.parse(date2);
                            Date parsedTime2 = timeFormat.parse(time2);

                            int dateComparison = parsedDate1.compareTo(parsedDate2);
                            if (dateComparison != 0) {
                                return dateComparison;
                            } else {
                                return parsedTime1.compareTo(parsedTime2);
                            }
                        } catch (ParseException e) {
                            e.printStackTrace();
                            return 0;
                        }
                    }
                });

                for (DataSnapshot sampleSnapshot : sortedSnapshots) {
                    String date = sampleSnapshot.child("date").getValue(String.class);
                    String time = sampleSnapshot.child("time").getValue(String.class);
                    Float tdsValue = sampleSnapshot.child("tdsValue").getValue(Float.class);
                    Float phValue = sampleSnapshot.child("phValue").getValue(Float.class);
                    Float AirTemperature = sampleSnapshot.child("AirTemperature").getValue(Float.class);
                    Float AirHumidity = sampleSnapshot.child("AirHumidity").getValue(Float.class);

                    if (date != null && time != null && tdsValue != null && phValue != null) {
                        Map<String, String> sampleData = new HashMap<>();
                        sampleData.put("date", date);
                        sampleData.put("time", time);
                        sampleData.put("tdsValue", String.valueOf(tdsValue));
                        sampleData.put("phValue", String.valueOf(phValue));
                        sampleData.put("AirTemperature", String.valueOf(AirTemperature));
                        sampleData.put("AirHumidity", String.valueOf(AirHumidity));

                        sampleDataList.add(sampleData);
                    }
                }

                SampleDataAdapter adapter = new SampleDataAdapter(HistoricalViewActivity.this, sampleDataList);
                sampleDataListView.setAdapter(adapter);
            }


            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                // Handle any errors
            }
        });
    }
}
