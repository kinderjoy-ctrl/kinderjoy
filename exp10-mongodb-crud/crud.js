const { MongoClient } = require('mongodb');

const url = 'mongodb://localhost:27017';
const dbName = 'crudDemoDB';

async function main() {
    const client = new MongoClient(url);

    try {
        await client.connect();
        console.log('Connected to MongoDB\n');

        const db = client.db(dbName);
        const col = db.collection('products');

        await col.deleteMany({});

        console.log('--- CREATE ---');
        var result = await col.insertMany([
            { name: 'Laptop', price: 999, category: 'Electronics' },
            { name: 'Phone', price: 699, category: 'Electronics' },
            { name: 'Desk Chair', price: 249, category: 'Furniture' },
            { name: 'Headphones', price: 149, category: 'Electronics' }
        ]);
        console.log('Inserted ' + result.insertedCount + ' documents\n');

        console.log('--- READ ALL ---');
        var all = await col.find({}).toArray();
        all.forEach(p => console.log('  ' + p.name + ' - $' + p.price));

        console.log('\n--- READ FILTERED (Electronics) ---');
        var filtered = await col.find({ category: 'Electronics' }).toArray();
        filtered.forEach(p => console.log('  ' + p.name + ' - $' + p.price));

        console.log('\n--- READ ONE ---');
        var one = await col.findOne({ name: 'Laptop' });
        console.log('  Found: ' + one.name + ' - $' + one.price + '\n');

        console.log('--- UPDATE ---');
        await col.updateOne({ name: 'Laptop' }, { $set: { price: 899 } });
        var updated = await col.findOne({ name: 'Laptop' });
        console.log('  Laptop new price: $' + updated.price + '\n');

        console.log('--- DELETE ---');
        await col.deleteOne({ name: 'Headphones' });
        console.log('  Deleted Headphones\n');

        console.log('--- FINAL STATE ---');
        var remaining = await col.find({}).toArray();
        remaining.forEach(p => console.log('  ' + p.name + ' - $' + p.price));

    } catch (err) {
        console.log('Error:', err);
    } finally {
        await client.close();
        console.log('\nDone.');
    }
}

main();
