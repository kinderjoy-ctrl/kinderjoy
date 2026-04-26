const express = require('express');
const mongoose = require('mongoose');
const app = express();

app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

mongoose.connect('mongodb://localhost:27017/cruddb')
    .then(() => console.log('Connected to MongoDB'))
    .catch(err => console.log('Error:', err));

const itemSchema = new mongoose.Schema({
    name: { type: String, required: true },
    description: String,
    createdAt: { type: Date, default: Date.now }
});

const Item = mongoose.model('Item', itemSchema);

app.post('/api/items', async (req, res) => {
    try {
        const item = new Item({ name: req.body.name, description: req.body.description });
        await item.save();
        res.json(item);
    } catch (err) {
        res.status(400).json({ error: err.message });
    }
});

app.get('/api/items', async (req, res) => {
    const items = await Item.find().sort({ createdAt: -1 });
    res.json(items);
});

app.put('/api/items/:id', async (req, res) => {
    const item = await Item.findByIdAndUpdate(req.params.id, req.body, { new: true });
    if (!item) return res.status(404).json({ error: 'Not found' });
    res.json(item);
});

app.delete('/api/items/:id', async (req, res) => {
    const item = await Item.findByIdAndDelete(req.params.id);
    if (!item) return res.status(404).json({ error: 'Not found' });
    res.json({ message: 'Deleted' });
});

app.listen(3000, () => console.log('Server running on http://localhost:3000'));
