package com.pothole_protector;



import static java.lang.Integer.parseInt;
import static java.lang.Long.parseLong;

import android.annotation.SuppressLint;

import android.content.Context;
import android.util.Log;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileFilter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;


public class FolderScanThread extends Thread {
    Context applicationContext;
    long mostRecentFile;
    FileFilter fileFilter;

    @SuppressLint("MissingPermission")
    public FolderScanThread(Context context) {
        applicationContext = context;
        fileFilter = new FileFilter() {
            public boolean accept(File dir) {
                if (dir.isFile()) {
                    return true;
                } else {
                    return false;
                }
            }
        };
    }

    @Override
    public void run() {
        while (true) {
            File[] potholeShit = new File("storage/emulated/0").listFiles(fileFilter);
            if (potholeShit != null) {
                for (File file : potholeShit) {
                    if (file.getName().contains("pothole_")) {
                        if(parseFile(file))
                        {
                            try {
                                Thread.sleep(5000);
                            } catch (InterruptedException e) {
                                throw new RuntimeException(e);
                            }
                        }
                    }
                }
            }
        }
    }

    private boolean parseFile(File file) {
        String[] fileName =  file.getName().split("[_.]");
        long fileNum;
        try {
            fileNum = parseLong(fileName[2]);
        } catch(NumberFormatException err) {
            return false;
        }
        if(fileNum <= mostRecentFile){
            return false;
        }
        mostRecentFile = fileNum;
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(file.getAbsolutePath()));
        } catch (FileNotFoundException error){
            Log.e("PARSE FILE", "unable to open file");
        }
        String line = null;
        try {
            while ((line = br.readLine()) != null)   //returns a Boolean value
            {
                String[] employee = line.split(",");


                //String timeStamp = employee[0];
                //String latitude = employee[1];
                //String longitude = employee[2];
                //String image = employee[3];
                //String image = employee[3];
                //String image = employee[3];
                //String image = employee[3];
                //String potholeProb = employee[6];
                //String image = employee[7];

                //get all fields



                // send to database


                // update map with markers for each pothole
            }
        } catch (IOException err){
            Log.e("PARSE FILE", "unable to read file");
        }
            return true;
    }
}