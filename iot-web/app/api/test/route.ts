import { NextResponse } from 'next/server';

export async function POST(request: Request) {
    try {
        // Parse the JSON body
        const body = await request.json();
        const { distance } = body;

        // TODO: Store data in sql

        return NextResponse.json({ success: true, data: { distance } });
    } catch (error) {
        return NextResponse.json({ success: false, error: 'Failed to process request' }, { status: 500 });
    }
}

export function OPTIONS() {
    return new Response(null, {
        status: 200,
        headers: {
            'Access-Control-Allow-Origin': '*',
            'Access-Control-Allow-Methods': 'POST, OPTIONS',
            'Access-Control-Allow-Headers': 'Content-Type',
        },
    });
}