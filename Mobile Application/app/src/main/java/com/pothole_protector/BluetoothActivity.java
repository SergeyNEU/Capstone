package com.pothole_protector;

import android.annotation.SuppressLint;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.MenuItem;
import android.webkit.WebView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.google.android.gms.maps.CameraUpdate;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.material.bottomnavigation.BottomNavigationMenuView;
import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.navigation.NavigationBarView;

import java.util.Set;

public class BluetoothActivity extends AppCompatActivity  implements OnMapReadyCallback {
    private static final String CHANNEL_ID = "com.pothole_protector_alerts";
    String piName;
    String piMacAddr;
    GoogleMap myMap = null;
    String curActiveFrag;
    BottomNavigationView menu;
    String user;

    Fragment map;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        Bundle b = getIntent().getExtras();
        user = b.getString("user");

        BluetoothManager bluetoothManager = getSystemService(BluetoothManager.class);
        BluetoothAdapter bluetoothAdapter = bluetoothManager.getAdapter();

        menu = findViewById(R.id.bottomNavigationView);
        menu.setOnItemSelectedListener(new NavigationBarView.OnItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                switch (item.getItemId()) {
                    case R.id.map:
                        FragmentManager fm = getSupportFragmentManager();
                        FragmentTransaction ft = fm.beginTransaction();
                        ft.remove(fm.findFragmentByTag(curActiveFrag));
                        ft.commit();
                        return true;

                    case R.id.manual:
                        curActiveFrag = "MANUAL_FRAGMENT";
                        switchToFragment(ManualDetectionFragment.newInstance(), "MANUAL_FRAGMENT");
                        return true;
                    case R.id.settings:
                        curActiveFrag = "SETTINGS_FRAGMENT";
                        Bundle args = new Bundle();
                        args.putString("user", user);
                        SettingsFragment f = SettingsFragment.newInstance();
                        f.setArguments(args);
                        switchToFragment(f, "SETTINGS_FRAGMENT");
                        return true;
                }
                return false;
            }
        });


        // bluetooth server is created in case we ever wanna update to connect stratight to app
        @SuppressLint("MissingPermission") Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            // There are paired devices. Get the name and address of each paired device.
            for (BluetoothDevice device : pairedDevices) {
                @SuppressLint("MissingPermission") String deviceName = device.getName();
                if (deviceName.equals("raspberrypi")) {
                    piName = deviceName;
                    piMacAddr = device.getAddress();
                    receiveData(bluetoothAdapter);
                }
            }
        }
        // Get a handle to the fragment and register the callback.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        map = mapFragment;

    }

    public void switchToFragment(Fragment fragment, String tag) {
        FragmentManager manager = getSupportFragmentManager();
        manager.beginTransaction().replace(R.id.map, fragment, tag).commit();
    }

    private void displayNotification(int i) {
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
        NotificationCompat.Builder builder = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setSmallIcon(R.drawable.logo)
                .setContentTitle("POTHOLE DETECTED")
                .setContentText("A pothole has been found and is being reported to the government")
                .setPriority(NotificationCompat.PRIORITY_DEFAULT);
        // ensure permissions
        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.POST_NOTIFICATIONS) != PackageManager.PERMISSION_GRANTED) {
            return;
        }
        notificationManager.notify(i, builder.build());
    }

    private void receiveData(BluetoothAdapter adapter) {
        AcceptThread thePi = new AcceptThread(adapter, getApplicationContext());
        thePi.start();
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = getString(R.string.channel_name);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_HIGH;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

    // Get a handle to the GoogleMap object and display marker.
    @Override
    public void onMapReady(GoogleMap googleMap) {
        LatLng centerNortheastern =  new LatLng(42.3398, -71.0892);
        googleMap.addMarker(new MarkerOptions()
                .position(centerNortheastern));

        CameraUpdate center=
                CameraUpdateFactory.newLatLng(centerNortheastern);
        CameraUpdate zoom= CameraUpdateFactory.zoomTo(10);
        googleMap.moveCamera(zoom);
        googleMap.moveCamera(center);
        myMap = googleMap;

        // start scanning for files and create notification channel
        createNotificationChannel();
        FolderScanThread heartbeat = new FolderScanThread(getApplicationContext());
        heartbeat.start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}