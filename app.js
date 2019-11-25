// Base requirements for the service to start
const express = require('express')
const expressLayouts = require('express-ejs-layouts')
const path = require('path')

// create instance of express service
const app = express()

// Set the port for the service
// process.env.PORT is needed for binding on Heroku
const port = process.env.PORT || 9001

// Set the views and public folder location.
app.set('views', path.join(__dirname, 'views'))
app.use(express.static(path.join(__dirname, 'public'), { maxAge: 31557600000 }))

// Tell the app middleware to use expressLayouts for rendering pages
app.use(expressLayouts)
app.set('view engine', 'ejs')

// Easier handling of post requests
app.use(express.json())
app.use(express.urlencoded({ extended: true }))

// Create base object so it's easier to manage later
app.locals.cadence = {}
app.locals.cadence.ride = []

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