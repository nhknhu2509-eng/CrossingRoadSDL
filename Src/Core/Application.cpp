// ==================================================
// GAME STATE UI
// ==================================================

if (!textureManager.LoadTexture(
    window.GetRenderer(),
    "pause_frame",
    "Assets/Images/UI/State/pause_frame.png"))
{
    std::cout
        << "Cannot load pause frame!"
        << std::endl;

    return false;
}


if (!textureManager.LoadTexture(
    window.GetRenderer(),
    "win_frame",
    "Assets/Images/UI/State/win_frame.png"))
{
    std::cout
        << "Cannot load win frame!"
        << std::endl;

    return false;
}


if (!textureManager.LoadTexture(
    window.GetRenderer(),
    "lose_frame",
    "Assets/Images/UI/State/lose_frame.png"))
{
    std::cout
        << "Cannot load lose frame!"
        << std::endl;

    return false;
}