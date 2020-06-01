// Import packages
import React from 'react'
import io from 'socket.io-client'

// Import components
import HelloWorld from './HelloWorld'

// Define component class
export default class App extends React.Component {
    constructor(props) {
        super(props)

        this.socket = io('http://localhost:3000')
        this.socket.on('connect', () => console.log('Socket connected!'))

        this.state = {

        }
    }

    render() {
        return (
            <div>
                <HelloWorld socket={this.socket}/>
            </div>
        )
    }
}
