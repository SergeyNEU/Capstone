package com.pothole_protector.database.devices;

import static androidx.room.ForeignKey.NO_ACTION;

import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.ForeignKey;
import androidx.room.Index;
import androidx.room.PrimaryKey;

import com.pothole_protector.database.user.User;

@Entity(indices = {@Index(value = {"deviceID"},unique = true)}, foreignKeys = {@ForeignKey(
        entity = User.class,
        parentColumns = { "userID"},
        childColumns = { "userID"},
        onDelete = NO_ACTION
)})
public class Devices {
    @PrimaryKey
    public int id;

    @ColumnInfo
    public String deviceID;

    @ColumnInfo
    public String userID;

    public Devices(String deviceID, String userID){
        this.deviceID = deviceID;
        this.userID = userID;
    }

}
