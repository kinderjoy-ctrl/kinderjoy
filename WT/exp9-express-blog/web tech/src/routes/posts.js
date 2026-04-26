const express = require('express');
const { randomUUID } = require('crypto');
const store = require('../services/postsStore');

const router = express.Router();

router.get('/', (_req, res) => {
  const posts = store.getAllPosts();
  res.render('posts/index', { posts });
});

router.get('/new', (_req, res) => {
  res.render('posts/new', { post: { title: '', body: '' }, error: null });
});

router.post('/', (req, res, next) => {
  try {
    const title = req.body.title?.trim();
    const body = req.body.body?.trim();

    if (!title || !body) {
      return res.status(400).render('posts/new', {
        post: { title: req.body.title || '', body: req.body.body || '' },
        error: 'Title and body are required.'
      });
    }

    store.createPost({
      id: randomUUID(),
      title,
      body,
      author: req.body.author?.trim() || 'Anonymous',
      createdAt: new Date().toISOString(),
      updatedAt: new Date().toISOString()
    });

    res.redirect('/posts');
  } catch (error) {
    next(error);
  }
});

router.get('/:id', (req, res, next) => {
  try {
    const post = store.getPostById(req.params.id);

    if (!post) {
      return res.status(404).render('errors/not-found', {
        title: 'Post not found',
        message: 'The post you are looking for does not exist.'
      });
    }

    res.render('posts/show', { post });
  } catch (error) {
    next(error);
  }
});

router.get('/:id/edit', (req, res, next) => {
  try {
    const post = store.getPostById(req.params.id);

    if (!post) {
      return res.status(404).render('errors/not-found', {
        title: 'Post not found',
        message: 'You cannot edit a post that does not exist.'
      });
    }

    res.render('posts/edit', { post, error: null });
  } catch (error) {
    next(error);
  }
});

router.put('/:id', (req, res, next) => {
  try {
    const existingPost = store.getPostById(req.params.id);

    if (!existingPost) {
      return res.status(404).render('errors/not-found', {
        title: 'Post not found',
        message: 'You cannot update a post that does not exist.'
      });
    }

    const title = req.body.title?.trim();
    const body = req.body.body?.trim();

    if (!title || !body) {
      return res.status(400).render('posts/edit', {
        post: {
          ...existingPost,
          title: req.body.title || '',
          body: req.body.body || ''
        },
        error: 'Title and body are required.'
      });
    }

    const updatedPost = store.updatePost(req.params.id, {
      title,
      body,
      author: req.body.author?.trim() || existingPost.author,
      updatedAt: new Date().toISOString()
    });

    res.redirect(`/posts/${updatedPost.id}`);
  } catch (error) {
    next(error);
  }
});

router.delete('/:id', (req, res, next) => {
  try {
    const removed = store.deletePost(req.params.id);

    if (!removed) {
      return res.status(404).render('errors/not-found', {
        title: 'Post not found',
        message: 'You cannot delete a post that does not exist.'
      });
    }

    res.redirect('/posts');
  } catch (error) {
    next(error);
  }
});

module.exports = router;
