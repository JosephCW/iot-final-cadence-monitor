/**
 * @index.js - manages all routing
 * 
 * router.get when single request
 * router.use when sending to a different controller
 * 
 * @requires express
 */

const express = require('express')

// Create instance of an express router
const router = express.Router()

// Manage top-level requests
router.get('/', (req, res, next) => {
    console.log('Request to /')
    res.render('index.ejs', { title: 'Bicycle Cadence Monitor' })
})

module.exports = router