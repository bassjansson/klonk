// Express

const express = require('express')
const app = express()
const port = process.env.PORT || 3000
const server = app.listen(port, () => console.log("Listening on port", port))

app.use(express.static('public'))


// MQTT

var mqtt = require('mqtt')
var client = mqtt.connect('mqtt://localhost')

client.on('connect', () => {
    console.log("Connected to MQTT broker.")

    // client.subscribe('testtopic', err => {
    //     if (!err) {}
    // })
})

// client.on('message', (topic, message) => {
//     // message is Buffer
//     console.log(topic, message.toString())
//     client.end()
// })


// Socket.io

const io = require('socket.io').listen(server)
const arduinoEvents = require('./arduino-events.js')

io.on('connect', socket => {
    console.log("New client connected!")

    socket.on('msg', msg => {
        if (msg.led) arduinoEvents.sendLedValue(msg.led.value)
    })

    socket.on('audio-control', msg =>
        client.publish('audio-control', `${msg.key},${msg.value}`))
})

arduinoEvents.on('pot-meter', value =>
    io.sockets.emit('msg', { pot: { value } }))
