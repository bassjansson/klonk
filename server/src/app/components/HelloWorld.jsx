// Import packages
import React from 'react'
import { Container, Button } from 'react-bootstrap'

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faLightbulb } from '@fortawesome/free-solid-svg-icons'

// Define component class
export default class HelloWorld extends React.Component {
    constructor(props) {
        super(props)

        this.state = {
            led: false
        }
    }

    turnLedOnOff = (e) => {
        let newLedState = !this.state.led

        //this.props.socket.emit('msg', { led: newLedState })

        this.setState({ led: newLedState })
    }

    render() {
        return (
            <Container className='border m-4 p-4'>
                <Button onClick={this.turnLedOnOff}>
                    {this.state.led ? 'Turn LED off' : 'Turn LED on'}
                </Button>
                <FontAwesomeIcon
                    className='fa-icon h2'
                    icon={faLightbulb}
                    style={{color: this.state.led ? 'orange' : 'lightgrey' }}/>
            </Container>
        )
    }
}
