const path = require('path');
const express = require('express');
const morgan = require('morgan');
const methodOverride = require('method-override');
const postsRouter = require('./routes/posts');
const { notFoundHandler, errorHandler } = require('./middleware/errors');

const app = express();
const port = process.env.PORT || 3000;

app.set('view engine', 'ejs');
app.set('views', path.join(__dirname, '..', 'views'));

app.use(express.urlencoded({ extended: false }));
app.use(express.static(path.join(__dirname, '..', 'public')));
app.use(morgan('dev'));
app.use(methodOverride('_method'));

app.locals.siteName = 'Writerly';

app.get('/', (_req, res) => {
  res.redirect('/posts');
});

app.get('/health', (_req, res) => {
  res.json({ status: 'ok' });
});

app.use('/posts', postsRouter);
app.use(notFoundHandler);
app.use(errorHandler);

app.listen(port, () => {
  console.log(`Writerly is running on http://localhost:${port}`);
});
