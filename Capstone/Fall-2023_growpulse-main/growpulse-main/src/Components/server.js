const express = require('express');
const nodemailer = require('nodemailer');
const { google } = require('googleapis');
const app = express();
app.use(express.json());

const OAuth2 = google.auth.OAuth2;

const oauth2Client = new OAuth2(
    "412920959953-0dgccdevi6vacqaqg0vdabnk9oi8ddes.apps.googleusercontent.com", // Client ID
    "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", // Client Secret
    "https://developers.google.com/oauthplayground" // Redirect URL
);

oauth2Client.setCredentials({
    refresh_token: "1//04V5iDXp0A0S_CgYIARAAGAQSNwF-L9Ir707udq6gkW_YmxLwWoSD2PEwBUWI_CuHOlAHYq1nM-srQwd8lpkLeDTdw8uQ1aIB_-w"
});

async function sendEmail(req, res) {
    try {
        const accessToken = await oauth2Client.getAccessToken();

        const transporter = nodemailer.createTransport({
            service: 'gmail',
            auth: {
                type: 'OAuth2',
                user: 'growpulse.help@gmail.com',
                accessToken: accessToken.token,
                clientId: "412920959953-0dgccdevi6vacqaqg0vdabnk9oi8ddes.apps.googleusercontent.com",
                clientSecret: "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",
                refreshToken: "1//04V5iDXp0A0S_CgYIARAAGAQSNwF-L9Ir707udq6gkW_YmxLwWoSD2PEwBUWI_CuHOlAHYq1nM-srQwd8lpkLeDTdw8uQ1aIB_-w"
            }
        });

        const { email, subject, description } = req.body;

        const mailOptions = {
            from: 'growpulse.help@gmail.com',
            to: 'growpulse.team@gmail.com',
            subject: `Message from ${email}: ${subject}`,
            text: description,
        };

        const result = await transporter.sendMail(mailOptions);
        console.log('Email sent: ' + result.response);
        res.status(200).send('Email sent');
    } catch (error) {
        console.error('Failed to send email', error);
        res.status(500).send('Failed to send email');
    }
}

app.post('/send-email', sendEmail);

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
});
