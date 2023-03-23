package com.pothole_protector.database.potholes;

import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.PrimaryKey;
import static androidx.room.ForeignKey.NO_ACTION;

import com.pothole_protector.database.devices.Devices;

@Entity(foreignKeys = {@ForeignKey(
        entity = Devices.class,
        parentColumns = { "deviceID"},
        childColumns = { "deviceAddress"},
        onDelete = NO_ACTION
)})
public class Potholes {
    @PrimaryKey
    public int uid;

    @ColumnInfo
    public String timeStamp;

    @ColumnInfo
    public double latitude;

    @ColumnInfo
    public double longitude;

    @ColumnInfo
    public double probPothole;

    @ColumnInfo
    public String imageStr;

    @ColumnInfo
    public String deviceAddress;

    public Potholes(String timeStamp, double latitude, double longitude, double probPothole, String imageStr, String deviceAddress)
    {
        this.timeStamp = timeStamp;
        this.latitude = latitude;
        this.longitude = longitude;
        this.probPothole = probPothole;
        this.imageStr = imageStr;
        this.deviceAddress = deviceAddress;
    }

}
