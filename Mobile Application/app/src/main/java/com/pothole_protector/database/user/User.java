package com.pothole_protector.database.user;

import androidx.room.ColumnInfo;
import androidx.room.Entity;
import androidx.room.Index;
import androidx.room.PrimaryKey;

@Entity(indices = {@Index(value = {"username"},unique = true)})
public class User {
    @PrimaryKey(autoGenerate = true)
    public int userID;

    @ColumnInfo
    public String username;

    @ColumnInfo
    public String password;

    @ColumnInfo
    public String deviceID;

    public User(String username, String password){
        this.username = username;
        this.password = password;

    }

}
