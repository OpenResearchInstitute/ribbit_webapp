# Ribbit Web App

This is the official repo for the Ribbit Web App

![Ribbit Icon](/ribbiticon-512.png)

Welcome to the Ribbit Web App!

The Ribbit Web App is an open-source project developed by the Open Research Institute. It is designed for both developers and ham radio enthusiasts who are interested in exploring the source code that was presented at DEFCON.

With the Ribbit Web App, you can dive deep into the codebase and inspect every aspect of the application. Whether you are a seasoned developer looking to contribute or a ham radio enthusiast curious about the inner workings, this app provides a platform for you to explore and learn.

By accessing the source code, you can gain insights into the innovative techniques and technologies used in the Ribbit Web App. You can also contribute to the project by submitting bug reports, suggesting improvements, or even submitting your own code enhancements.

The code base is entirely written in the index.html file, the only external javascript file is the service-worker.js for caching files for off-line operation and the ribbit.wasm which listens to the microphone and decodes any incoming signals. There are also a bunch of icons that mobile devices seem to love.

---

For more information about how Ribbit came to be check out [this repo](https://github.com/OpenResearchInstitute/ribbit) and of course you can read up on the official [web site](https://ribbitradio.org).

## ribbit.wasm

The most important part of the app is the web assembly. To build this you'll need to be a bit familiar with [emscripten](https://emscripten.org/) and the c++ file `ribbit.cc`  located in the webassembly directory. Slight modifications were made based on the [Ribbit Android](https://github.com/OpenResearchInstitute/Ribbit_Android) repo created by [Ahmet Inan](https://github.com/xdsopl) to interface with javascript.

You'll see some `EXTERN EMSCRIPTEN_KEEPALIVE` prefixing some functions in the cc file, this allows javascript to access those functions. I left a few test functions in there just to keep parity with what was shown at DEFCON, but I'll be working on cleaning the test code out with future releases.

## index.html

This is a relatively large file, but for a very simple reason. Avoiding external dependencies, libraries, frameworks and files. With the exception of the service-worker and wasm files, everything required for the app is in the index.html; fonts included.

It's not that I have anything against frameworks like node, electron etc. For novice devs with just a little CSS and HTML experience the flat html structure should be easier to play with.

Monolithic files tend to get awkward to work with, but in this case I figured the best way to reduce file complexity was to avoid referencing too many files.

## PWA status

Once some experimentation is done I'll add in the "install app" button to the settings panel. I'll also want to add an instructions page for general use as well as how to share the page. I'll be adding some of that functionality soon, I promise.

### settings

![settings](/media/settings.png)

The settings panel still needs some work, there are some known bugs here.

![settings](/media/chat.png)

The chat window also requires some work, so if you have specific thoughts please create a new Issue.

## Next Steps

After addressing some of the simple CSS problems, I'll be creating a compact header system. Right now the character buffer being transmitted has a bunch of text taken up by the call sign and gps coordinates etc. Rather than use UTF-8 for all the characters it'll be simplified to a smaller character set and use many fewer bits to transmit the information.

## Developer Info

If you're interested in more regular updates I post regularly [here](https://ham.okita.ninja/)
