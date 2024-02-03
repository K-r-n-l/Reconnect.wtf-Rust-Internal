#include "Update.h"


inline AssemblyCSharp::ExplosionsFPS* Instance = 0;
inline AssemblyCSharp::ExplosionsFPS* Instance2 = 0;

void Hooks::Update_(AssemblyCSharp::MainMenuSystem* a1)
{

	if (Instance == 0)
		Instance = reinterpret_cast<AssemblyCSharp::ExplosionsFPS*>(a1->get_gameObject()->AddComponent(System::Type::PostProcessDebug()));

	return UpdateFN(a1);
}