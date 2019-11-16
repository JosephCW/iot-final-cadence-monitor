/**
 * Cadence controller
 * Handle requests related to cadence
 * get / - ejs render of statistics
 * get /startRide - ends a ride, just send you back to root page
 * get /stopRide - ends a ride, redirects back to root page
 * get /statistics - .json rendering of the calculated ride statistics
 * post /addReading - post request { currentTime: <seconds past epoch>, strokesSinceLastPublish: <5>}
 * 
 * 
 */

// req.app.locals.~~

 const express = require('express')
 const api = express.Router()

 api.get('/', (req, res) => {
     console.log('At /cadence/')
     res.render('cadence/index.ejs')
 })

 // Note the start time and assign it to the app's ride['startTime'] property
 api.get('/startRide', (req, res) => {
     console.log('At /cadence/startRide')
     const currentDate = new Date()
     const secondsSinceEpoch = Math.round(currentDate.getTime() / 1000)
     req.app.locals.cadence.ride.startTime = secondsSinceEpoch
     console.log(`Start Time: ${req.app.locals.cadence.ride.startTime}`)
     res.redirect('/cadence/')
 })

 // Note the stop time and assign it to the app's ride['endTime'] property
 api.get('/stopRide', (req, res) => {
    console.log('At /cadence/startRide')
    const currentDate = new Date()
    const secondsSinceEpoch = Math.round(currentDate.getTime() / 1000)
    req.app.locals.cadence.ride.stopTime = secondsSinceEpoch
    req.app.locals.cadence.ride.duration = parseInt(req.app.locals.cadence.ride.stopTime) - parseInt(req.app.locals.cadence.ride.startTime)
    console.log(`Stop Time: ${req.app.locals.cadence.ride.stopTime}`)
    console.log(`Ride Duration: ${req.app.locals.cadence.ride.duration}`)
    res.redirect('/cadence/')
})

 module.exports = api