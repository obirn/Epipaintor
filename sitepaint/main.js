var tl = gsap.timeline();
tl.from(".nav", {
  opacity: 0,
  x: "-10%",
  duration: 1.5,
});
tl.from(
  ".hero-text",
  {
    y: "-200%",
    opacity: 0,
    duration: 2,
    ease: Power4.easeOut,
  },
  "-=1"
);
tl.from(
  ".btn",
  {
    opacity: 0,
    duration: 0.5,
  },
  "-=1.5"
);

const sr = ScrollReveal({
  origin: "top",
  distance: "80px",
  duration: 1000,
  opacity: 0,
  reset: true,
});

sr.reveal("#skill-text", {});
sr.reveal(".iteration", { delay: 200 });

function showResponsiveMenu() {
  var menu = document.getElementById("topnav_responsive_menu");
  var icon = document.getElementById("topnav_hamburger_icon");
  var root = document.getElementById("root-nav");
  if (menu.className === "") {
    menu.className = "open";
    icon.className = "open";
    root.style.overflowY = "hidden";
  } else {
    menu.className = "";
    icon.className = "";
    root.style.overflowY = "";
  }
}

//Get the button:
mybutton = document.getElementById("backtotop");

// When the user scrolls down 20px from the top of the document, show the button
window.onscroll = function () {
  scrollFunction();
};

function scrollFunction() {
  if (document.body.scrollTop > 20 || document.documentElement.scrollTop > 20) {
    mybutton.style.display = "flex";
  } else {
    mybutton.style.display = "none";
  }
}

// When the user clicks on the button, scroll to the top of the document
function topFunction() {
  document.body.scrollTop = 0; // For Safari
  document.documentElement.scrollTop = 0; // For Chrome, Firefox, IE and Opera
}
