package com.pothole_protector;

import android.os.Bundle;

import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;

import android.view.View;
import android.widget.Button;

import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

import com.pothole_protector.databinding.ActivityBluetoothBinding;

public class BluetoothActivity extends AppCompatActivity {

    Button blueButt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        blueButt = findViewById(R.id.connect);
        blueButt.setOnClickListener(l -> {connectToRaspberry();});


    }

    private void connectToRaspberry() {
    }

}