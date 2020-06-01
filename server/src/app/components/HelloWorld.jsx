// Import packages
import React from 'react'
import { Button } from 'react-bootstrap'

import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faCoffee } from '@fortawesome/free-solid-svg-icons'
import { faFontAwesome } from '@fortawesome/free-brands-svg-icons'

// Define component class
export default class HelloWorld extends React.Component {
    constructor(props) {
        super(props)

        this.state = {
            text: "Hello World"
        }
    }

    render() {
        return (
            <div>
                <p>{`${this.state.text} ${this.props.name}! :)`}</p>
                <Button onClick={e => this.setState({ text: "You Rock!" })}>
                    Click Me
                </Button>
                <p style={{ fontSize: '24px' }}>
                    With some
                    <FontAwesomeIcon className='fa-icon' icon={faCoffee} spin />
                    we got amazing icons to work! Thank you
                    <FontAwesomeIcon className='fa-icon' icon={faFontAwesome} transform={{ rotate: 20 }} />
                </p>
            </div>
        )
    }
}
