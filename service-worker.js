const CACHE_NAME = 'ribbit-cache-v7';  // bump on every release
const urlsToCache = [
    '/',
    '/index.html',
    '/ribbit.wasm',
    '/ribbit.webmanifest',
    '/ribbiticon-32.png',
    '/ribbiticon-64.png',
    '/ribbiticon-128.png',
    '/ribbiticon-256.png',
    '/ribbiticon-512.png'
];

self.addEventListener('install', event => {
    event.waitUntil(
        caches.open(CACHE_NAME).then(cache => cache.addAll(urlsToCache))
    );
});

// Delete caches left over from earlier versions
self.addEventListener('activate', event => {
    event.waitUntil(
        caches.keys().then(names =>
            Promise.all(names.filter(n => n !== CACHE_NAME).map(n => caches.delete(n)))
        )
    );
});

self.addEventListener('fetch', event => {
    event.respondWith(
        caches.match(event.request).then(response => response || fetch(event.request))
    );
});
