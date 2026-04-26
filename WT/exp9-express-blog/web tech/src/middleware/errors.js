function notFoundHandler(_req, res) {
  res.status(404).render('errors/not-found', {
    title: 'Page not found',
    message: 'The page you requested could not be found.'
  });
}

function errorHandler(error, _req, res, _next) {
  console.error(error);
  res.status(500).render('errors/error', {
    title: 'Server error',
    message: 'Something went wrong while processing your request.'
  });
}

module.exports = {
  notFoundHandler,
  errorHandler
};
