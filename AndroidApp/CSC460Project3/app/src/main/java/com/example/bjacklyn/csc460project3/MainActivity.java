package com.example.bjacklyn.csc460project3;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;

import android.util.Log;
import android.widget.ImageView;
import android.widget.Toast;

import java.util.Set;


public class MainActivity extends ActionBarActivity implements View.OnTouchListener {

    // Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;

    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    private static final String TAG = MainActivity.class.getSimpleName();

    private static final int REQUEST_ENABLE_BT = 1;
    private static final int CENTER_PADDING = 50;

    private static final float centerX = 350;
    private static final float centerY = 350;

    private Handler bluetoothHandler;
    private BluetoothRunnable bluetoothRunnable;
    private BluetoothAdapter bluetoothAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ImageView img = (ImageView) findViewById(R.id.imageView);
        img.setImageResource(R.drawable.target);
        img.setOnTouchListener(this);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null)
        {
            Log.d(TAG, "BluetoothAdapter is null");
        }
    }

    @Override
    protected void onStart() {
        super.onStart();

        if (!bluetoothAdapter.isEnabled()) {
            Log.d(TAG, "Bluetooth is not enabled");
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        } else {
            bluetoothHandler = new Handler();
            bluetoothRunnable = new BluetoothRunnable(bluetoothHandler, bluetoothAdapter, this.getBaseContext(), this.mHandler);

            bluetoothHandler.post(bluetoothRunnable);
        }
    }

    @Override
    protected void onStop() {
        // cleanup activity
        bluetoothHandler.removeCallbacks(bluetoothRunnable);
        bluetoothRunnable = null;

        super.onStop();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_ENABLE_BT) {
            if (resultCode != Activity.RESULT_OK) {
                Log.d(TAG, "Bluetooth was not enabled by the user");
            } else {
                bluetoothHandler = new Handler();
                bluetoothRunnable = new BluetoothRunnable(bluetoothHandler, bluetoothAdapter, this.getBaseContext(), this.mHandler);

                bluetoothHandler.post(bluetoothRunnable);
            }
        }
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN || event.getAction() == MotionEvent.ACTION_MOVE) {
            float curX = event.getX();
            float curY = event.getY();
            updateRoomba(curX, curY);
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            updateRoomba(centerX, centerY);
        } else {
            // do nothing
        }

        return true;
    }

    private void updateRoomba(float curX, float curY) {
        float speed = 0;
        if (curY > (centerY + CENTER_PADDING) || curY < (centerY - CENTER_PADDING)) { // + 25? make 25 a variable
            speed = (curY - centerY) * (-1);
            speed /= centerY;

            if (speed > 1)
                speed = 1;
            else if (speed < -1)
                speed = -1;
        }

        float rotate = 0;
        if (curX > (centerX + CENTER_PADDING) || curX < (centerX - CENTER_PADDING)) {
            rotate = (curX - centerX);
            rotate /= centerX;

            if (rotate > 1)
                rotate = 1;
            else if (rotate < -1)
                rotate = -1;
        }

        bluetoothRunnable.updateRoombaCommand(speed, rotate);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    // The Handler that gets information back from the BluetoothChatService
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MESSAGE_STATE_CHANGE:
                      Log.d(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
//                    switch (msg.arg1) {
//                        case BluetoothChatService.STATE_CONNECTED:
//                            mTitle.setText(R.string.title_connected_to);
//                            mTitle.append(mConnectedDeviceName);
//                            mConversationArrayAdapter.clear();
//                            break;
//                        case BluetoothChatService.STATE_CONNECTING:
//                            mTitle.setText(R.string.title_connecting);
//                            break;
//                        case BluetoothChatService.STATE_LISTEN:
//                        case BluetoothChatService.STATE_NONE:
//                            mTitle.setText(R.string.title_not_connected);
//                            break;
//                    }
                    break;
                case MESSAGE_WRITE:
                    byte[] writeBuf = (byte[]) msg.obj;
                    // construct a string from the buffer
                    String writeMessage = new String(writeBuf);
                    break;
                case MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    // construct a string from the valid bytes in the buffer
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    break;
                case MESSAGE_DEVICE_NAME:
                    // save the connected device's name
                    String mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                    Toast.makeText(getApplicationContext(),
                            "Connected to " + mConnectedDeviceName,
                            Toast.LENGTH_SHORT).show();
                    break;
                case MESSAGE_TOAST:
                    Toast.makeText(getApplicationContext(),
                            msg.getData().getString(TOAST), Toast.LENGTH_SHORT)
                            .show();
                    break;
            }
        }
    };
}
