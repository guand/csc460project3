package com.example.bjacklyn.csc460project3;


import static java.lang.Math.*;

/**
 * Created by Bjacklyn on 4/3/2015.
 */
public class RoombaCommand {
    private int speed;
    private int rotate;

    public RoombaCommand(float speed, float rotate) {
        this.speed = (int) floor(speed * 10) + 75;
        this.rotate = (int) floor(rotate * 10) + 75;
    }

    public void update(float speed, float rotate) {
        this.speed = (int) floor(speed * 10) + 75;
        this.rotate = (int) floor(rotate * 10) + 75;
    }

    public int getSpeed() {
        return speed;
    }

    public int getRotate() {
        return rotate;
    }
}
