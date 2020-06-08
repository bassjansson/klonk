// Require event emmitter and arduino class
const EventEmitter = require('events').EventEmitter
const Arduino = require('./arduino.js')

// Create an arduino events instance
const arduinoEvents = new EventEmitter()

// Create the arduino instances
const piPort = '/dev/ttyAMA0'
const macPort = '/dev/cu.usbmodem14201'
const testArduino = new Arduino(macPort)


//========================================//
//========== Control Data Sends ==========//
//========================================//

arduinoEvents.sendLedValue = value => {
    testArduino.sendControlData({
        control: 100, // CONTROL_LED
        data: [value ? 1 : 0]
    })
}


//========================================//
//========== Sensor Data Events ==========//
//========================================//

testArduino.receiveSensorData(sensorData => {
    if (sensorData.sensor == 200) { // SENSOR_POT
        let value = sensorData.data[0] / 1023.0

        if (value < 0.0) value = 0.0
        if (value > 1.0) value = 1.0

        arduinoEvents.emit('pot-meter', Math.floor(value * 100.0))
    }
})


//===================================//
//========== Export Module ==========//
//===================================//

module.exports = arduinoEvents
