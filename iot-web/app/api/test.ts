import { NextApiRequest, NextApiResponse } from 'next';

export default function handler(req: NextApiRequest, res: NextApiResponse) {
    res.setHeader('Access-Control-Allow-Origin', '*'); // Allow all origins
    res.setHeader('Access-Control-Allow-Methods', 'POST');
    res.setHeader('Access-Control-Allow-Headers', 'Content-Type');
    if (req.method === 'POST') {
        // Parse the request body
        const { name, message } = req.body;

        // Perform your logic here (e.g., store in a database, send an email, etc.)

        res.status(200).json({ success: true, data: { name, message } });
    } else {
        // Handle any other HTTP method (GET, PUT, etc.)
        res.setHeader('Allow', ['POST']);
        res.status(405).end(`Method ${req.method} Not Allowed`);
    }
}
