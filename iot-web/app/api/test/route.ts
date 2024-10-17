import { NextResponse } from 'next/server';
import { PrismaClient } from '@prisma/client';

async function storeDistance(distance: number, deviceId: number) {
    const prisma = new PrismaClient();
    const record = await prisma.movementRecord.create({
        data: {
            distance: distance,
            deviceId: deviceId,
        },
    });
    await prisma.$disconnect();
    console.log('Stored distance:', record);
}

export async function POST(request: Request) {
    try {
        // Parse the JSON body
        const body = await request.json();
        const { distance, deviceId } = body;

        console.log("Reciving distance:", distance);

        storeDistance(distance, deviceId);


        return NextResponse.json({ success: true });
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