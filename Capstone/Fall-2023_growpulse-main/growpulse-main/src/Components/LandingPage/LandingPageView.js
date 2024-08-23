//LandingPageView.js
import React, { useEffect, useRef, useState } from 'react';
import './LandingPage.css';
import logo from '../Images/GrowpulseLogos/growpulse-align-left.png';
import GrowPulseIcon from '../Images/GrowpulseLogos/growpulse-leaf-black.png';
import headerLogo from '../Images/GrowpulseLogos/growpulse-leaf-white.png'
import RegisterModal from "../RegisterModal/RegisterModal";
import { features, descriptions } from './LandingPageModel';
import { useLandingPageController } from './LandingPageController';
import GreenConcordia from '../Images/LandingPageIcons/green_concordia.png';
import Concordia from '../Images/LandingPageIcons/Concordia-logo.png';
import SAF from '../Images/LandingPageIcons/SAF-Logo.png';
import Firebase from '../Images/LandingPageIcons/Firebase_Logo.png';
import AWS from '../Images/LandingPageIcons/aws-logo.png';

function LandingPageView() {
    const introSectionRef = useRef(null);
    const mainSectionRef = useRef(null);
    const servicesSectionRef = useRef(null);
    const aboutUsSectionRef = useRef(null);
    const contactUsSectionRef = useRef(null);
    const [isRegisterModalOpen, setIsRegisterModalOpen] = useState(false);
    const [animating, setAnimating] = useState(false);
    const featuresLengthRef = useRef(features.length);
    const {
        startFeatureIndex,
        setStartFeatureIndex,
        isHeaderMinimized,
        showScrollTop,
        scrollToSection,
        scrollToFeatureDescription,
        handleScroll
    } = useLandingPageController();

    useEffect(() => {
        const interval = setInterval(() => {
            setAnimating(`disappearing`); // Start disappearing animation

            setTimeout(() => {
                setStartFeatureIndex((prevIndex) => (prevIndex + 5) % features.length); // Change icons
                setAnimating('appearing'); // Start appearing animation
            }, 300); // Start change after 300ms

            setTimeout(() => setAnimating(false), 600); // End animation after another 300ms
        }, 3500);

        return () => clearInterval(interval);
    }, [setStartFeatureIndex]);

    useEffect(() => {
        window.addEventListener('scroll', handleScroll);
        return () => window.removeEventListener('scroll', handleScroll);
    }, [handleScroll]);

    useEffect(() => {
        featuresLengthRef.current = features.length;
    }, []);


    return (
        <div className="landing-container">
            <section ref={introSectionRef} className="intro-section">
                <header className={`landing-header ${isHeaderMinimized ? 'minimized-header' : ''}`}>
                    <img src={isHeaderMinimized ? headerLogo : logo} alt="Logo" className="logo" />
                    <nav className="navigation">
                        <a href={"#top"} className="nav-link" onClick={(e) => { e.preventDefault(); scrollToSection(introSectionRef); }}>Welcome</a>
                        <a href={"#features"} className="nav-link" onClick={(e) => { e.preventDefault(); scrollToSection(mainSectionRef); }}>Features</a>
                        <a href={"#about"} className="nav-link" onClick={(e) => { e.preventDefault(); scrollToSection(aboutUsSectionRef); }}>About Us</a>
                        <a href={"#contact"} className="nav-link" onClick={(e) => { e.preventDefault(); scrollToSection(contactUsSectionRef); }}>Contact Us</a>
                        <button className="get-started-button" onClick={() => setIsRegisterModalOpen(true)}>Get Started</button>
                    </nav>
                </header>
                <div className="title-container">
                    <img src={GrowPulseIcon} alt="GrowPulse Icon" className="GrowPulse-icon" />
                    <div className="overlay-text">
                        <h1 className="landing-title">Harvesting Innovation</h1>
                        <p className="landing_description">
                        Revolutionizing Agriculture with AI-Driven Hydroponic Farming Solutions. Experience the future of farming - smarter, easier, and more efficient.
                        </p>
                    </div>
                </div>
                <div className="scroll-down-container" onClick={() => scrollToSection(mainSectionRef)}>
                    <div className="double-arrow" />
                    <p className="scroll-text">Scroll Down</p>
                </div>
            </section>
            <section ref={mainSectionRef} className="main-section">
                <div className="about-autobud-container">
                    <div className="features-banner">
                        {features.slice(startFeatureIndex, startFeatureIndex + 5).map((feature, index) => (
                            <div key={index} className={`feature-item`} data-animating={animating} onClick={() => scrollToFeatureDescription(index)}>
                                <img src={feature.src} alt={feature.alt} className="feature-icon" />
                                <p>{feature.text}</p>
                            </div>
                        ))}
                    </div>
                </div>
            </section>
            <section ref={servicesSectionRef} className="services-section">
                <div className="features-wrapper">
                    {features.map((feature, index) => (
                        <div key={index} className="feature-row">
                            <div className={`feature-description ${index % 2 === 0 ? 'right' : 'left'}`}>
                                <h3>{feature.text}</h3>
                                <p>{descriptions[index]?.description || ''}</p>
                            </div>
                            <div className="individual-feature">
                                <img src={feature.src} alt={feature.alt} className="individual-feature-icon" />
                            </div>
                        </div>
                    ))}
                    <div className="split-line"></div>
                </div>
            </section>
            <section ref={aboutUsSectionRef} className="aboutUs-section">
                <div className="sponsors-section">
                    <h2>Our Partners & Sponsors</h2>
                    <div className="sponsors-logos">
                        <img src={GreenConcordia} alt="Green Concordia" className="sponsor-logo GreenConcordia" />
                        <img src={Concordia} alt="Concordia University" className="sponsor-logo Concordia" />
                        <img src={SAF} alt="Sustainability Action Fund" className="sponsor-logo SAF" />
                        <img src={Firebase} alt="Firebase" className="sponsor-logo Firebase" />
                        <img src={AWS} alt="Amazon Web Services" className="sponsor-logo AWS" />
                    </div>
                </div>
            </section>
            <section ref={contactUsSectionRef} className="contactUs-section">
                <h2 className="contactUs-title">Contact Us</h2>
                <p>We would love to hear from you! Please fill out the form below and we'll get in touch shortly.</p>
                <form className="contact-form">
                    <input type="text" placeholder="Your Name" className="contact-input" required />
                    <input type="email" placeholder="Your Email" className="contact-input" required />
                    <textarea placeholder="Your Message" rows="5" className="contact-input" required></textarea>
                    <button type="submit" className="contact-submit">Send Message</button>
                </form>
            </section>
            <div
                className={`back-to-top ${showScrollTop ? 'show' : ''}`}
                onClick={() => window.scrollTo({ top: 0, behavior: 'smooth' })}>
                ⌃
            </div>
            <RegisterModal
                isOpen={isRegisterModalOpen}
                onRequestClose={() => setIsRegisterModalOpen(false)}
            />
        </div>
    );
}

export default LandingPageView;
