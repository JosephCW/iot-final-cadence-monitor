// Base requirements for the service to start
const express = require('express')
const expressLayouts = require('express-ejs-layouts')
const path = require('path')

// create instance of express service
const app = express()

// Set the port and hostname for the service
const port = 9001

// Set the views and public folder location.
app.set('views', path.join(__dirname, 'views'))
app.use(express.static(path.join(__dirname, 'public'), { maxAge: 31557600000 }))

// Tell the app middleware to use expressLayouts for rendering pages
app.use(expressLayouts)
app.set('view engine', 'ejs')

// Run all routing through routes/index.js
const routes = require('./routes/index.js')
app.use('/', routes)

// For any unknown page, render 404.ejs
app.use((req, res) => { res.status(404).render('404.ejs') })

// Tell the service to actually start listening
app.listen(port, () => {
    console.log(`Bicycle Cadence Service running on port ${port}`)
})

module.exports = app