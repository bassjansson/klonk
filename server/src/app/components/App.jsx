// Import packages
import React from 'react'

// Import components
import HelloWorld from './HelloWorld'

// Define component class
export default class App extends React.Component {
    constructor(props) {
        super(props)

        this.state = {
            name: "Klonk"
        }
    }

    render() {
        return (
            <div>
                <HelloWorld name={this.state.name}/>
            </div>
        )
    }
}
