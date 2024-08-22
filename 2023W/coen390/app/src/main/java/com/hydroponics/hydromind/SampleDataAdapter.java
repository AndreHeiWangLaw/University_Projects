package com.hydroponics.hydromind;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import java.util.List;
import java.util.Map;

public class SampleDataAdapter extends ArrayAdapter<Map<String, String>> {
    private final LayoutInflater inflater;
    private final List<Map<String, String>> sampleDataList;

    public SampleDataAdapter(@NonNull Context context, List<Map<String, String>> sampleDataList) {
        super(context, 0, sampleDataList);
        this.inflater = LayoutInflater.from(context);
        this.sampleDataList = sampleDataList;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        ViewHolder viewHolder;

        if (convertView == null) {
            convertView = inflater.inflate(R.layout.historical_list_item, parent, false);
            viewHolder = new ViewHolder();
            viewHolder.sampleDateTime = convertView.findViewById(R.id.sample_date_time);
            viewHolder.tdsValue = convertView.findViewById(R.id.tds_value);
            viewHolder.phValue = convertView.findViewById(R.id.ph_value);
            viewHolder.AirTemperature = convertView.findViewById(R.id.AirTemperature);
            viewHolder.AirHumidity = convertView.findViewById(R.id.AirHumidity);
            convertView.setTag(viewHolder);
        } else {
            viewHolder = (ViewHolder) convertView.getTag();
        }

        Map<String, String> sampleData = sampleDataList.get(position);

        String dateTime = sampleData.get("date") + " " + sampleData.get("time");
        viewHolder.sampleDateTime.setText(dateTime);
        viewHolder.tdsValue.setText("TDS: " + sampleData.get("tdsValue") + "ppm");
        viewHolder.phValue.setText("pH: " + sampleData.get("phValue"));
        viewHolder.AirTemperature.setText("Temperature: " + sampleData.get("AirTemperature") + "°C");
        viewHolder.AirHumidity.setText("Humidity: " + sampleData.get("AirHumidity") + "%");

        return convertView;
    }

    private static class ViewHolder {
        TextView sampleDateTime;
        TextView tdsValue;
        TextView phValue;
        TextView AirTemperature;
        TextView AirHumidity;
    }
}
