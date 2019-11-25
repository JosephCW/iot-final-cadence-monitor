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

     const newRideId = req.app.locals.cadence.ride.length
     const ride = {
        id: newRideId,
        readings: [],
        startTime: secondsSinceEpoch,
        stopTime: 0,
        duration: 0,
        mean: 0,
        median: 0,
        currentCadence: 0
    }
    req.app.locals.cadence.ride.push(ride)
    console.log(`Start Time: ${req.app.locals.cadence.ride[newRideId].startTime}`)
    res.json(req.app.locals.cadence.ride[newRideId])
    //res.redirect('/cadence/')
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

// Send a json version of the current cadence object
api.get('/statistics', (req, res) => {
    console.log('At /cadence/statistics')
    res.json(req.app.locals.cadence)
})

// Helper function for once data has been added.
function updateStatistics(app) {
    // Add up all of the pedal strokes for the ride
    let sumOfStrokes = 0
    app.locals.cadence.ride.readings.forEach((reading) => {
        sumOfStrokes += parseInt(reading.strokesSinceLastPublish)
    })
    //console.log(`Sum of strokes: ${sumOfStrokes}`)
    // Get the start time
    const startTime = app.locals.cadence.ride.startTime
    // If the ride is already over then use the stop time
    // if the ride is still going, then use the current time as the stop time
    const currentDate = new Date()
    const currentTimeInSeconds = Math.round(currentDate.getTime() / 1000)
    const stopTime = app.locals.cadence.ride.stopTime != 0 ? 
                        app.locals.cadence.ride.stopTime :
                        currentTimeInSeconds
    
    // Calculate mean using above variables
    app.locals.cadence.ride.mean = (sumOfStrokes / (stopTime - startTime)) * 60


    // Calculate current cadence
    const numReadings = app.locals.cadence.ride.readings.length
    // If there is more than one reading
    // Time between the starting and ending reading is > 5 seconds
    // Find the minimum span to add up to 5 seconds
    // extrapolate time / sum of strokes
    console.log(`numReadings: ${numReadings}`)
    if (numReadings > 1) {
        // find how many elements it takes to get a reading duration >= 5 seconds
        let sumOfReadingTime = 0
        sumOfStrokes = 0
        let i = numReadings - 1
        while (i > 0) {
            // add on the timing difference between the previous two readings.
            const rideDifference = app.locals.cadence.ride.readings[i].currentTime - app.locals.cadence.ride.readings[i-1].currentTime
            console.log(`rideDifference: ${rideDifference}`)
            sumOfReadingTime += rideDifference
            sumOfStrokes += app.locals.cadence.ride.readings[i].strokesSinceLastPublish
            console.log(`sumOfReadingTime: ${sumOfReadingTime}`)
            console.log(`sumOfStrokes: ${sumOfStrokes}`)
            if (sumOfReadingTime >= 5) {
                break;
            }
            i -= 1
        }
        app.locals.cadence.ride.currentCadence = (sumOfStrokes / sumOfReadingTime) * 60
    }
}

// Handle the post request to add data to the ride, recalc stats
api.post('/addReading', (req, res) => {
    console.log('posted to /addReading')
    const currentTime = parseInt(req.body.currentTime)
    const strokesSinceLastPublish = parseInt(req.body.strokesSinceLastPublish)
    req.app.locals.cadence.ride.readings.push({"currentTime": currentTime, "strokesSinceLastPublish": strokesSinceLastPublish})
    updateStatistics(req.app)
    res.end(`Added new reading ct: ${currentTime}, sSLP: ${strokesSinceLastPublish}, mean: ${req.app.locals.cadence.ride.mean}`)
})

 module.exports = api