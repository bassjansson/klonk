const express = require('express')
const app = express()
const port = process.env.PORT || 3000
const server = app.listen(port, () => console.log("Listening on port", port))
const io = require('socket.io').listen(server)

app.use(express.static('public'))

io.on('connect', socket => {
    console.log("New client connected!")

    socket.on('msg', msg => console.log(msg))
})
