package com.pothole_protector;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import android.widget.Button;

import java.util.Set;

public class BluetoothActivity extends AppCompatActivity {

    Button blueButt;
    String piName;
    String piMacAddr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        blueButt = findViewById(R.id.connect);

        BluetoothManager bluetoothManager = getSystemService(BluetoothManager.class);
        BluetoothAdapter bluetoothAdapter = bluetoothManager.getAdapter();

        @SuppressLint("MissingPermission") Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            // There are paired devices. Get the name and address of each paired device.
            for (BluetoothDevice device : pairedDevices) {
                @SuppressLint("MissingPermission") String deviceName = device.getName();
                if(deviceName.equals("raspberrypi")){
                    piName = deviceName;
                    piMacAddr = device.getAddress();
                    receiveData(bluetoothAdapter);
                }
            }
        }
        FolderScanThread heartbeat = new FolderScanThread(getApplicationContext());
        heartbeat.start();

    }
    private void receiveData(BluetoothAdapter adapter){
        AcceptThread thePi = new AcceptThread(adapter, getApplicationContext());
        thePi.start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}