/**
 * Cadence controller
 * Handle requests related to cadence
 */

 const express = require('express')
 const api = express.Router()

 api.get('/', (req, res) => {
     console.log('At /cadence/')
     console.log('Handling GET / ')
     res.render('cadence/index.ejs')
 })

 module.exports = api