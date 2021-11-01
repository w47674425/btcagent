package main

type EventType uint8

type EventApplicationExit struct{}

type EventInitFinished struct{}

type EventUpSessionReady struct {
	Slot    int
	Session *UpSession
}

type EventUpSessionInitFailed struct {
	Slot int
}

type EventAddStratumSession struct {
	Session *StratumSession
}

type EventConnBroken struct{}

type EventRecvExMessage struct {
	Message *ExMessage
}

type EventRecvJSONRPC struct {
	RPCData   *JSONRPCLine
	JSONBytes []byte
}

type EventSendBytes struct {
	Content []byte
}

type EventStratumSessionBroken struct {
	SessionID uint32
}

type EventUpSessionBroken struct {
	Slot int
}