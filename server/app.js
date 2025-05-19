const express = require('express');
const cors = require('cors');
const fs = require('fs');
const path = require('path');

const app = express();
const port = process.env.PORT || 3000;
const DB_FILE_PATH = path.join(__dirname, 'dice_history.json');

app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

function readHistoryFromFile() {
    try {
        if (fs.existsSync(DB_FILE_PATH)) {
            const jsonData = fs.readFileSync(DB_FILE_PATH, 'utf-8');
            if (jsonData.trim() === '') {
                return [];
            }
            return JSON.parse(jsonData);
        }
        return [];
    } catch (error) {
        console.error('Error reading from history file:', error);
        return [];
    }
}

function writeHistoryToFile(historyData) {
    try {
        const jsonData = JSON.stringify(historyData, null, 2);
        fs.writeFileSync(DB_FILE_PATH, jsonData, 'utf-8');
    } catch (error) {
        console.error('Error writing to history file:', error);
    }
}

let rollHistory = readHistoryFromFile();

app.post('/api/dice_roll', (req, res) => {
    console.log('Received data from ESP32:', req.body);
    let rollValue;

    if (req.body && req.body.roll !== undefined) {
        rollValue = req.body.roll;
    } else {
        return res.status(400).json({ message: 'Roll data not found in expected format.' });
    }

    const parsedRoll = parseInt(rollValue);

    if (isNaN(parsedRoll) || parsedRoll < 1 || parsedRoll > 6) {
        return res.status(400).json({ message: 'Invalid roll value. Must be an integer between 1 and 6.' });
    }

    const newRoll = {
        roll_value: parsedRoll,
        timestamp: new Date().toISOString()
    };

    rollHistory.unshift(newRoll);

    const MAX_HISTORY_ITEMS = 50;
    if (rollHistory.length > MAX_HISTORY_ITEMS) {
        rollHistory = rollHistory.slice(0, MAX_HISTORY_ITEMS);
    }

    writeHistoryToFile(rollHistory);

    console.log('New roll added and history saved:', newRoll);
    res.status(201).json({ message: 'Dice roll received and saved successfully', data: newRoll });
});

app.get('/api/history', (req, res) => {
    res.status(200).json(rollHistory);
});

app.get('/', (req, res) => {
    res.send('IoT Dice Roller Backend (using JSON file) is running!');
});

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
    console.log(`Storing history in: ${DB_FILE_PATH}`);
});