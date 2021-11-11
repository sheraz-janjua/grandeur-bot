/*
    main.js
    Initialize the SDK and get
    a reference to the project
*/
var canvas, ctx;
var project = grandeur.init("grandeurkv7jxe7700af0k178y0f1xcf", "accesskvnwixok03jh0ixfefx1hftc", "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltRmpZMlZ6YzJ0MmJuZHBlRzlyTUROcWFEQnBlR1psWm5neGFHWjBZeUlzSW5SNWNHVWlPaUpoWTJObGMzTWlMQ0pwWVhRaU9qRTJNell5TURnNE9EZDkuZ0ZwTjFIMzhLaDZiaHBPa3FZbGpFdXNSVW1YNVZCSzM5T3NUczE5NlM3VSJ9");
var devices;
var deviceID = "devicekvrmkbrf0a9m0ixff2hj4phy";
var timer = null;
var connected = false;
/* Setting the connection status update handler */
project.onConnection((status) => {
    /* 
        This callback gets fired
        whenever the connection status
        changes
    */

    switch (status) {
        case "CONNECTED":
            /*
                If SDK is connected,
                we set the status.
            */
            document.getElementById("status").innerText = "Connected";
            devices = project.devices();
            connected = true;
            break;
        default:
            /* If SDK gets disconnected, we display the status
               on the app and clear the timer.
             */
            document.getElementById("status").innerText = "Disconnected";
            connected = false;
            /* Clears timer */
            clearInterval(timer);
    }
});

/* Function to login user */
async function login() {
    /* Store credentials into variables */
    var email = "user@somemail.com";
    var password = "userpass";

    /* Set the status to logging in */
    document.getElementById("status").innerText = "Logging in";

    /* Then get a reference to auth class */
    var auth = project.auth();

    /* and in a try catch block */
    try {
        /* Submit request using login function */
        var res = await auth.login(email, password);

        /* 
            Got the response to login
            handle response
        */
        switch (res.code) {
            case "AUTH-ACCOUNT-LOGGEDIN":
            case "AUTH-ACCOUNT-ALREADY-LOGGEDIN":
                /*
                    User Authenticated
                    Set the status to success
                */
                document.getElementById("status").innerText = "User Authenticated";
                break;

            default:
                /* 
                    Logging failed due
                    to invalid data
                */
                document.getElementById("status").innerText = "Authentication Failed";
                if (res.code == "AUTH-ACCOUNT-ALREADY-LOGGEDIN") dispay_joystick();
        }
    }
    catch (err) {
        /*
            Error usually got generated when
            we are not connected to the internet
        */
        document.getElementById("status").innerText = "Network Error";
    }
}

window.addEventListener('load', () => {

    canvas = document.getElementById('canvas');
    ctx = canvas.getContext('2d');
    resize();

    document.addEventListener('mousedown', startDrawing);
    document.addEventListener('mouseup', stopDrawing);
    document.addEventListener('mousemove', Draw);

    document.addEventListener('touchstart', startDrawing);
    document.addEventListener('touchend', stopDrawing);
    document.addEventListener('touchcancel', stopDrawing);
    document.addEventListener('touchmove', Draw);
    window.addEventListener('resize', resize);
});




var width, height, x_orig, y_orig;
const radius = 100;
function send_xy(x, y) {
    // TODO: send data 
    const speed = Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2)) / 10; // 0 to 1
    var sr = 0, sl = 0;
    if (x > 0) {
        sl = Math.ceil(x * speed);
        sr = Math.ceil((100 - x) * speed);
    }
    else if (x < 0) {
        sl = Math.ceil((100 + x) * speed);
        sr = Math.ceil((-x) * speed);
    }
    const f = y < 0 ? 'F' : y > 0 ? 'B' : '*';

    console.log('x = ' + x + ', y = ' + y + '\n');
    console.log('sl = ' + sl + ', sr = ' + sr + '\n');
    console.log('speed = ' + speed + ', f= ' + f + '\n');
    if (devices && connected) {
        devices.device(deviceID).data().set("f", f);
        devices.device(deviceID).data().set("sl", sl.toString());
        devices.device(deviceID).data().set("sr", sr.toString());
    }
}
function resize() {
    width = window.innerWidth;
    height = radius * 6.5;
    ctx.canvas.width = width;
    ctx.canvas.height = height;
    background();
    joystick(width / 2, height / 3);
}

function background() {
    x_orig = width / 2;
    y_orig = height / 3;

    ctx.beginPath();
    ctx.arc(x_orig, y_orig, radius + 20, 0, Math.PI * 2, true);
    ctx.fillStyle = '#ECE5E5';
    ctx.fill();
}

function joystick(width, height) {
    ctx.beginPath();
    ctx.arc(width, height, radius, 0, Math.PI * 2, true);
    ctx.fillStyle = '#000000';
    ctx.fill();
    ctx.strokeStyle = '#D1CFCF';
    ctx.lineWidth = 10;
    ctx.stroke();
    send_xy(0, 0);
}

let coord = { x: 0, y: 0 };
let paint = false;

function getPosition(event) {
    var mouse_x = event.clientX ? event.clientX : event.touches[0].clientX ? event.touches[0].clientX : 0;
    var mouse_y = event.clientY || event.touches[0].clientY;
    coord.x = mouse_x - canvas.offsetLeft;
    coord.y = mouse_y - canvas.offsetTop;
}

function is_it_in_the_circle() {
    var current_radius = Math.sqrt(Math.pow(coord.x - x_orig, 2) + Math.pow(coord.y - y_orig, 2));
    if (radius >= current_radius) return true
    else return false
}


function startDrawing(event) {
    paint = true;
    getPosition(event);
    if (is_it_in_the_circle()) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        background();
        joystick(coord.x, coord.y);
        Draw();
    }
}


function stopDrawing() {
    paint = false;
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    background();
    joystick(width / 2, height / 3);
}

function Draw(event) {

    if (paint) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        background();
        var x, y;
        var angle = Math.atan2((coord.y - y_orig), (coord.x - x_orig));


        if (is_it_in_the_circle()) {
            joystick(coord.x, coord.y);
            x = coord.x;
            y = coord.y;
        }
        else {
            x = radius * Math.cos(angle) + x_orig;
            y = radius * Math.sin(angle) + y_orig;
            joystick(x, y);
        }


        getPosition(event);

        var x_relative = Math.round(x - x_orig);
        var y_relative = Math.round(y - y_orig);
        var f = (y_relative > 0) ? 'F' : '*';
        send_xy(x_relative, y_relative);
    }
}
/* Call login on startup */
login();