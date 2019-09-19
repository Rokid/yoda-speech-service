#include "flora-cli.h"
#include "infinite-voice.h"
#include "rlog.h"

#define TAG "speech-service.demo"
#define FLORA_URI "tcp://localhost:37800/#speech-service.demo"

using namespace std;

int main(int argc, char** argv) {
	shared_ptr<flora::Client> cli;
	InfiniteVoice iv;
	
	if (flora::Client::connect(FLORA_URI, &iv, 0, cli) != FLORA_CLI_SUCCESS) {
		KLOGE(TAG, "connect to flora %s failed", FLORA_URI);
		return 1;
	}

	cli->subscribe("rokid.speech.final_asr");
	cli->subscribe("rokid.speech.nlp");
	cli->subscribe("rokid.speech.error");

	shared_ptr<Caps> msg = Caps::new_instance();
	// speech server uri
	msg->write("wss://cloudapigw.open.rokid.com:443/api");
	// key
	msg->write("6DDECE40ED024837AC9BDC4039DC3245");
	// device type id
	msg->write("B16B2DFB5A004DCBAFD0C0291C211CE1");
	// secret
	msg->write("F2A1FDC667A042F3A44E516282C3E1D7");
	// device id
	msg->write("speech-service.demo");
	// reconn interval
	msg->write(10000);
	// ping interval
	msg->write(10000);
	// no resp timeout
	msg->write(20000);
  // conn duration
  msg->write(900);
	cli->post("rokid.speech.prepare_options", msg, FLORA_MSGTYPE_PERSIST);

	msg = Caps::new_instance();
	// LAGN ZH
	msg->write((int32_t)0);
	// CODEC PCM
	msg->write((int32_t)0);
	// VadMode CLOUD
	msg->write((int32_t)1);
	// vad timeout
	msg->write((int32_t)700);
	// no nlp: default
	msg->write((int32_t)-1);
	// no intermediate asr
	msg->write((int32_t)0);
	// vad begin
	msg->write((int32_t)0);
  // voice fragment
  msg->write(0x10000000);
	cli->post("rokid.speech.options", msg, FLORA_MSGTYPE_PERSIST);

	iv.run(cli);
	return 0;
}
