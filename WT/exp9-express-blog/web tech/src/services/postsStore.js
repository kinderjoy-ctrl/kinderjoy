const fs = require('fs');
const path = require('path');

const dataDir = path.join(__dirname, '..', '..', 'data');
const dataFile = path.join(dataDir, 'posts.json');

function ensureDataFile() {
  if (!fs.existsSync(dataDir)) {
    fs.mkdirSync(dataDir, { recursive: true });
  }

  if (!fs.existsSync(dataFile)) {
    const seedPosts = [
      {
        id: 'seed-post-1',
        title: 'Welcome to Writerly',
        body: 'This is your first post. You can edit it, delete it, or replace it with your own content.',
        author: 'System',
        createdAt: new Date('2026-04-26T12:00:00.000Z').toISOString(),
        updatedAt: new Date('2026-04-26T12:00:00.000Z').toISOString()
      }
    ];

    fs.writeFileSync(dataFile, JSON.stringify(seedPosts, null, 2));
  }
}

function readPosts() {
  ensureDataFile();
  const raw = fs.readFileSync(dataFile, 'utf8');

  try {
    return JSON.parse(raw);
  } catch {
    return [];
  }
}

function writePosts(posts) {
  ensureDataFile();
  fs.writeFileSync(dataFile, JSON.stringify(posts, null, 2));
}

function getAllPosts() {
  return readPosts().sort((left, right) => new Date(right.updatedAt) - new Date(left.updatedAt));
}

function getPostById(id) {
  return readPosts().find((post) => post.id === id) || null;
}

function createPost(post) {
  const posts = readPosts();
  posts.unshift(post);
  writePosts(posts);
  return post;
}

function updatePost(id, updates) {
  const posts = readPosts();
  const index = posts.findIndex((post) => post.id === id);

  if (index === -1) {
    return null;
  }

  posts[index] = {
    ...posts[index],
    ...updates
  };

  writePosts(posts);
  return posts[index];
}

function deletePost(id) {
  const posts = readPosts();
  const filteredPosts = posts.filter((post) => post.id !== id);

  if (filteredPosts.length === posts.length) {
    return false;
  }

  writePosts(filteredPosts);
  return true;
}

module.exports = {
  getAllPosts,
  getPostById,
  createPost,
  updatePost,
  deletePost
};
