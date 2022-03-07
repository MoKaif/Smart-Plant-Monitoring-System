const loginElement = document.querySelector("#login-form");
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector("#user-details");
const authBarElement = document.querySelector("#authentication-bar");

// Elements for sensor readings
const tempElement = document.getElementById("temp");
const humElement = document.getElementById("hum");
const soilElement = document.getElementById("soil");

// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = "none";
    contentElement.style.display = "block";
    authBarElement.style.display = "block";
    userDetailsElement.style.display = "block";
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from database
    var uid = user.uid;
    console.log(uid);

    // Database paths (with user UID)
    var dbPathTemp = "UsersData/" + uid.toString() + "/temperature";
    var dbPathHum = "UsersData/" + uid.toString() + "/humidity";
    var dbPathSoil = "UsersData/" + uid.toString() + "/SoilMoistureContent";

    // Database references
    var dbRefTemp = firebase.database().ref().child(dbPathTemp);
    // console.log("Hello" + dbRefTemp);
    var dbRefHum = firebase.database().ref().child(dbPathHum);
    var dbRefSoil = firebase.database().ref().child(dbPathSoil);

    // Update page with new readings
    dbRefTemp.on("value", (snap) => {
      console.log("this val" + snap.val());
      tempElement.innerText = snap.val();
    });

    dbRefHum.on("value", (snap) => {
      humElement.innerText = snap.val().toFixed(2);
      console.log(snap.val().toFixed(2));
    });

    dbRefSoil.on("value", (snap) => {
      soilElement.innerText = snap.val().toFixed(2);
      console.log(snap.val().toFixed(2))
    });

    // if user is logged out
  } else {
    // toggle UI elements
    loginElement.style.display = "block";
    authBarElement.style.display = "none";
    userDetailsElement.style.display = "none";
    contentElement.style.display = "none";
  }

};
