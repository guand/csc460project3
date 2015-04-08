package com.example.bjacklyn.csc460project3;

/**
 * Created by Bjacklyn on 4/3/2015.
 */

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.os.Handler;
import android.util.Log;

import java.util.Set;

public class BluetoothRunnable implements Runnable {

    private RoombaCommand roombaCommand = new RoombaCommand(0, 0);
    private static final String TAG = MainActivity.class.getSimpleName();
    private static final String ROOMBA_BLUETOOTH_NAME = "RoombaBluetooth";

    private Handler bluetoothHandler;
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothChatService bluetoothChatService;

    private Context activityContext;
    private Handler activityHandler;

    public BluetoothRunnable(Handler bluetoothHandler, BluetoothAdapter bluetoothAdapter, Context activityContext, Handler activityHandler) {
        super();

        this.bluetoothHandler = bluetoothHandler;
        this.bluetoothAdapter = bluetoothAdapter;

        this.activityContext = activityContext;
        this.activityHandler = activityHandler;
    }

    @Override
    public void run() {
        connect();
        loop();
    }

    public void connect() {
        if (bluetoothChatService == null) {
            Log.d(TAG, "Attempting to connect to bluetooth device");
            bluetoothChatService = new BluetoothChatService(activityContext, activityHandler);

            Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();

            for (BluetoothDevice device : pairedDevices) {
                Log.d(TAG, device.getName());
                if (device.getName().equals(ROOMBA_BLUETOOTH_NAME)) {
                    bluetoothChatService.connect(device);
                    Log.d(TAG, "Connected to bluetooth device");
                }
            }
        }
    }

    // send packet with command to roomba
    public void loop() {
        synchronized (roombaCommand) {
            // send packet
            Log.d(TAG, "speed: " + roombaCommand.getSpeed());
            Log.d(TAG, "rotate: " + roombaCommand.getRotate());

            String msg = Character.toString((char) roombaCommand.getSpeed()) + Character.toString((char) roombaCommand.getRotate());
            Log.d(TAG, msg);
            bluetoothChatService.write(msg.getBytes());
        }

        bluetoothHandler.postDelayed(this, 500); // 500 => delayMilliseconds
    }

    public void updateRoombaCommand(float speed, float rotate) {
        synchronized (roombaCommand) {
            roombaCommand.update(speed, rotate);
        }
    }
}
