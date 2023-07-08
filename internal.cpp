



#define FRICTION 0.83
#define SCALE 20.0f
#define TIMEOUT 50.0f



void drawGird2D(float2 spacing, float thickness, float2 offset)
{
    for(int i = 0; i < resolution.x / spacing.x; i ++)
    {
        for(int j = 0; j < resolution.y / spacing.y; j ++)
        {
            DrawRectangle((i * spacing.x + (i * thickness)), (j * spacing.y + (j * thickness)), spacing.x, spacing.y, GRAY);
        }
    }
}


struct CAM2D
{
    float2 position;
};










struct Killer
{
    float health = 1.0f;
    float2 position;
    float2 oldPosition;
    float2 acceleration;
    int DamageCooldown = 0;


    void update(float dt = 1.0f)
    {
        const float2 velocity = position - oldPosition;

        oldPosition = position;

        position = position + velocity * 0.95f + acceleration * dt * dt;

        acceleration = {};
    }

    void teleport(float2 v)
    {
        position = v;
        oldPosition = v;
    }

    void teleport(float x, float y)
    {
        position = (float2) { x, y };
        oldPosition = position;
    }

    float2 getVelocity()
    {
        return position - oldPosition;
    }

    void Render()
    {
        DrawRectangle(position.x - SCALE, position.y - SCALE, SCALE * 2.0f, SCALE * 2.0f, (Color) { (unsigned char)(100 + int((float(DamageCooldown) / TIMEOUT) * 155)), (unsigned char)(235 + int((float(DamageCooldown) / TIMEOUT) * -200)), (unsigned char)(150 + int(((float)DamageCooldown / TIMEOUT) * -100)), 255 });

        if(DamageCooldown > 0)
        {   
            DamageCooldown --;

            // DrawRectangleLines(std :: max(position.x - SCALE * 1.5f, 0.0f), std :: max(position.y - SCALE * 2.0f, 0.0f), SCALE * 3.0f, SCALE * 0.5f, WHITE);
            // DrawRectangle(std :: max(position.x - SCALE * 1.5f, 0.0f) + 1, std :: max((position.y - SCALE * 2.0f), 0.0f) + 1, ((SCALE * 3.0f) - 2) * health, (SCALE * 0.5f) - 2, RED);
            DrawRectangleLines(std :: min(std :: max(position.x - SCALE * 1.5f, 0.0f), (resolution.x - SCALE * 3.0f) - 1.0f), std :: min((double)std :: max(position.y - SCALE * 2.0f, 0.0f), (resolution.y - SCALE * 4.0) - 1.0f), SCALE * 3.0f, SCALE * 0.5f, WHITE);
            DrawRectangle(std :: min((double)std :: max(position.x - SCALE * 1.5f, 0.0f) + 1.0f, (resolution.x - SCALE * 3.0)), std :: min((double)std :: max((position.y - SCALE * 2.0f), 0.0f) + 1, resolution.y - SCALE * 4.0), ((SCALE * 3.0f) - 2) * health, (SCALE * 0.5f) - 2, RED);
        }
    }

    void solvePals(std :: vector<Killer> *k, int index)
    {
        int dx;
        for(dx = 0; dx < k -> size(); dx ++)
        {
            Killer * i = &k -> at(dx);
            if(dx <= index) continue;

            Rectangle rec1 = (Rectangle) { position.x - SCALE, position.y - SCALE, SCALE * 2.0f, SCALE * 2.0f };
            Rectangle rec2 = (Rectangle) { i -> position.x - SCALE, i -> position.y - SCALE, SCALE * 2.0f, SCALE * 2.0f };

            if(!CheckCollisionRecs(rec1, rec2))
            {
                continue;
            }


            Rectangle rec = GetCollisionRec( rec1, rec2 );

            if(rec.width < rec.height)
            {
                position.x = (position.x < rec.x ? position.x - rec.width * 0.5f : position.x + rec.width * 0.5f); //- offset.height;
                i -> position.x = (i -> position.x < rec.x ? i -> position.x - rec.width * 0.5f : i -> position.x + rec.width * 0.5f); //- offset.height;
            }
            else
            {
                position.y = (position.y < rec.y ? position.y - rec.height : position.y + rec.height); //- offset.height;
                i -> position.y = (i -> position.y < rec.y ? i -> position.y - rec.height : i -> position.y + rec.height); //- offset.height;
            }
            

            dx ++;
        }
    }

    
    void limitArea()
    {
        if(position.y > resolution.y - SCALE)
        {
            position.y = resolution.y - SCALE;
        }

        else if(position.y < SCALE)
        {
            position.y = SCALE;
        }


        if(position.x > resolution.x - SCALE)
        {
            position.x = resolution.x - SCALE;
        }

        else if(position.x < SCALE)
        {
            position.x = SCALE;
        }
    }
};










struct Bot
{
    float health = 1.0f;
    int lockedTarget; 
    bool isLocked;
    float2 position;
    float2 oldPosition;
    float2 acceleration;
    
    void update(float dt = 1.0f)
    {
        const float2 velocity = position - oldPosition;

        oldPosition = position;

        position = position + velocity * FRICTION + acceleration * dt * dt;

        acceleration = {};
    }

    void teleport(float2 v)
    {
        position = v;
        oldPosition = v;
    }

    void teleport(float x, float y)
    {
        position = (float2) { x, y };
        oldPosition = position;
    }

    void Render(float angle)
    {
        DrawRectangle(position.x - SCALE, position.y - SCALE, SCALE * 2.0f, SCALE * 2.0f, WHITE);
        DrawLineEx(position.V(), (position + (float2){ cosf(angle), sinf(angle) } * 75.0f).V(), 5.0f, SKYBLUE);

        DrawRectangleLines(std :: min(std :: max(position.x - SCALE * 1.5f, 0.0f), (resolution.x - SCALE * 3.0f) - 1.0f), std :: min((double)std :: max(position.y - SCALE * 2.0f, 0.0f), (resolution.y - SCALE * 4.0) - 1.0f), SCALE * 3.0f, SCALE * 0.5f, WHITE);
        DrawRectangle(std :: min((double)std :: max(position.x - SCALE * 1.5f, 0.0f) + 1.0f, (resolution.x - SCALE * 3.0)), std :: min((double)std :: max((position.y - SCALE * 2.0f), 0.0f) + 1, resolution.y - SCALE * 4.0), ((SCALE * 3.0f) - 2) * health, (SCALE * 0.5f) - 2, BLUE);
    }

    void solvePals(std :: vector<Killer> *k)
    {
        int dx;
        for(dx = 0; dx < k -> size(); dx ++)
        {
            Killer * i = &k -> at(dx);

            Rectangle rec1 = (Rectangle) { position.x - SCALE, position.y - SCALE, SCALE * 2.0f, SCALE * 2.0f };
            Rectangle rec2 = (Rectangle) { i -> position.x - SCALE, i -> position.y - SCALE, SCALE * 2.0f, SCALE * 2.0f };

            if(!CheckCollisionRecs(rec1, rec2))
            {
                continue;
            }


            Rectangle rec = GetCollisionRec( rec1, rec2 );

            if(rec.width < rec.height)
            {
                position.x = (position.x < rec.x ? position.x - rec.width * 0.5f : position.x + rec.width * 0.5f); //- offset.height;
                i -> position.x = (i -> position.x < rec.x ? i -> position.x - rec.width * 0.5f : i -> position.x + rec.width * 0.5f); //- offset.height;
            }
            else
            {
                position.y = (position.y < rec.y ? position.y - rec.height : position.y + rec.height); //- offset.height;
                i -> position.y = (i -> position.y < rec.y ? i -> position.y - rec.height : i -> position.y + rec.height); //- offset.height;
            }
            

            dx ++;
        }
    }

    void limitArea()
    {
        if(position.y > resolution.y - SCALE)
        {
            position.y = resolution.y - SCALE;
        }

        else if(position.y < SCALE)
        {
            position.y = SCALE;
        }


        if(position.x > resolution.x - SCALE)
        {
            position.x = resolution.x - SCALE;
        }

        else if(position.x < SCALE)
        {
            position.x = SCALE;
        }
    }
};
















struct Particle
{
    float2 position;
    float2 oldPosition;
    float2 acceleration;
    float2 Friction = (float2) { 0.95f, 0.95f };

    void update(float dt = 1.0f)
    {
        const float2 velocity = position - oldPosition;

        oldPosition = position;

        position = position + velocity * Friction + acceleration * dt * dt;
    }

    float2 getVelocity()
    {
        return position - oldPosition;
    }

    void teleport(float x, float y)
    {
        position.x = x;
        position.y = y;

        oldPosition = position;
    }

    void teleport(float2 v)
    {
        position = v;
        oldPosition = v;
    }
};




struct ParticleSystem
{
    std :: vector<Particle> particles;
    std :: string TYPE;

    void update()
    {
        for(auto &v : particles)
        {
            v.update();
        }
    }

    void accelerate(float2 acceleration)
    {
        for(auto &v : particles)
        {
            v.acceleration += acceleration;
        }
    }

    void accelerate(float x, float y)
    {
        for(auto &v : particles)
        {
            v.acceleration += (float2) { x, y };
        }
    }
};

















struct Bullet
{
    float2 position;
    float2 oldPosition;
    float2 acceleration;


    void update(float dt = 1.0f)
    {
        const float2 velocity = position - oldPosition;

        oldPosition = position;

        position = position + velocity + acceleration * dt * dt;

        acceleration = {};
    }

    void teleport(float2 v)
    {
        position = v;
        oldPosition = v;
    }

    void teleport(float x, float y)
    {
        position = (float2) { x, y };
        oldPosition = position;
    }

    float2 getVelocity()
    {
        return position - oldPosition;
    }

    void Render()
    {
        const float2 velocity = normalized2(getVelocity());
        DrawLineEx(position.V(), (position + velocity * 50.0f).V(), 7.0f, YELLOW);
    }
};


struct VFX
{
    Shader shader;
    float rotation;
    float2 position;
    int rotationLoc;
    std :: vector<int> paramLocation;
    
    void Render(RenderTexture2D * target)
    {
        BeginShaderMode(shader);
            ClearBackground(BLACK);

            DrawTextureRec(target -> texture, (Rectangle){ 0, 0, (float)target -> texture.width, (float)-target -> texture.height }, (Vector2){ 0, 0 }, WHITE);

        EndShaderMode();
    }

    void RenderParticleArray(RenderTexture2D * target, RenderTexture2D * sampler, ParticleSystem * sys, int locIndex)
    {
        if(sys -> particles.size() < 1)
        {
            Render(target);
            return;
        }

        BeginTextureMode(*sampler);

            int d = 0;
            for(auto &s : sys -> particles)
            {
                DrawPixel(d, 0, (Color) { (unsigned char)((int) ((s.position.x / resolution.x) * 255.0f)), (unsigned char)((int) ((s.position.y / resolution.y) * 255.0f)), 0});

                d ++;
            }
            
        EndTextureMode();

        SetShaderValue(shader, paramLocation.at(locIndex), sampler, SHADER_UNIFORM_SAMPLER2D);

        Render(target);
    }

    void getLocation(std :: string names)
    {
        paramLocation.push_back(GetShaderLocation(shader, names.c_str()));
    }

    void setParameters(int locIndex, const void * v, int TYPE)
    {
        SetShaderValue(shader, paramLocation.at(locIndex), v, TYPE);
    }

    void getRotationLocation()
    {
        rotationLoc = GetShaderLocation(shader, "rotation");
    }
};





















CAM2D cam;


Bot bot;

float CursorAngle = PI * 0.5f;
float CursorDistanceScale = 100.0f;

std :: vector<Bullet> bullet;

std :: vector<Killer> killer;

std :: vector<VFX> vfx;


ParticleSystem sys;
















void start()
{
    bot.teleport(center);
    bot.acceleration += 1.0f;

    vfx.clear();
    
    vfx.push_back((VFX){ LoadShader(0, TextFormat("render.fs", GLSL_VERSION)) });

    vfx.at(0).getLocation("showBeam");
    vfx.at(0).getLocation("Xres");
    vfx.at(0).getLocation("Yres");
    vfx.at(0).getLocation("ppos");

    vfx.at(0).setParameters(0, &bot.isLocked, SHADER_UNIFORM_INT);

    vfx.at(0).setParameters(1, &resolution.x, SHADER_UNIFORM_FLOAT);
    vfx.at(0).setParameters(2, &resolution.y, SHADER_UNIFORM_FLOAT);

    vfx.at(0).getRotationLocation();

    sys = (ParticleSystem) {  };

    sys.TYPE = "BEAM";
}


void Loop(int Tick)
{

    if(bot.health <= 0.0f)
    {
        ShowCursor();
        return;
    }



    // For bot

    CursorAngle = atan2f(GetMouseY() - center.y, GetMouseX() - center.x);

    float2 direction = (float2) { cosf(CursorAngle), sinf(CursorAngle) };

    float CursorDistFromCenter = distance2((float2) { (float)GetMouseX(), (float)GetMouseY() }, center);

    if(CursorDistFromCenter > CursorDistanceScale || CursorDistFromCenter < CursorDistanceScale - 10.0f)
    {
        SetMousePosition(center.x + direction.x * CursorDistanceScale, center.y + direction.y * CursorDistanceScale);
    }

    if(IsKeyDown(KEY_W))
    {
        bot.acceleration += direction;
    }  
    if(IsKeyDown(KEY_S))
    {
        bot.acceleration -= direction;
    }

    if(IsKeyDown(KEY_A))
    {
        bot.acceleration -= (float2) { -direction.y, direction.x };
    }  
    if(IsKeyDown(KEY_D))
    {
        bot.acceleration += (float2) { -direction.y, direction.x };
    }
    if(IsMouseButtonDown(0) && Tick % 5 == 0)
    {
        Bullet b;
        b.teleport(bot.position);
        b.acceleration = direction * 20.0f;

        bullet.push_back(b);
    }

    bot.solvePals(&killer);

    bot.update();

    bot.limitArea();






    // For bullet

    int i = 0;
    for(auto &v : bullet)
    {
        if(v.getVelocity().length() < 0.3f && v.acceleration.length() < 0.3f)
        {
            bullet.erase(bullet.begin() + i);
            continue;
        }

        v.update();

        i ++;
    }




    // For killer



    if((Tick + 1) % 120 == 0)
    {
        Killer k;

        k.teleport(center);
        k.acceleration += normalized2(bot.position - k.position) * 0.1;

        killer.push_back(k);
    }


    RayCollision worldRayCast;
    worldRayCast.distance = 2000.0f;


    int _dx = 0;

    for(auto &v : killer)
    {
        v.acceleration += normalized2(bot.position - v.position) * 0.1;
        v.update();
        v.solvePals(&killer, _dx);
        v.limitArea();


        // If it the box is in view
        RayCollision r = GetRayCollisionBox((Ray) { (Vector3){ bot.position.x, bot.position.y, 0.0f }, (Vector3){ direction.x, direction.y, 0.0f } }, (BoundingBox) { (Vector3){ v.position.x - SCALE, v.position.y - SCALE, 0 }, (Vector3){ v.position.x + SCALE, v.position.y + SCALE, 0 } });

        if(IsMouseButtonDown(1))
        {
            if(r.hit && worldRayCast.distance > r.distance)
            {
                worldRayCast = r;
                bot.lockedTarget = _dx;
                bot.isLocked = true;
            }
        }


        int _bx = 0;
        for(auto &b : bullet)
        {
            if(b.position.x < v.position.x + SCALE + 4 && b.position.x > (v.position.x - 4) - SCALE && 
                b.position.y < v.position.y + SCALE + 4 && b.position.y > (v.position.y - 4) - SCALE)
                {
                    // killer.erase(killer.begin() + _dx);

                    v.acceleration += b.getVelocity() * 0.5f;
                    v.DamageCooldown = TIMEOUT;
                    v.health -= 0.06f;
                    bullet.erase(bullet.begin() + _bx);

                    if(v.health <= 0.0f)
                    {
                        if(_dx == bot.lockedTarget)
                        {
                            bot.isLocked = false;
                        }

                        killer.erase(killer.begin() + _dx);
                    }
                }
            _bx ++ ;
        }

        if(bot.position.x - SCALE < v.position.x + SCALE &&
        bot.position.x + SCALE > v.position.x - SCALE &&
        bot.position.y - SCALE < v.position.y + SCALE &&
        bot.position.y + SCALE > v.position.y - SCALE)
        {
            bot.health -= 0.01f;
        }

        _dx ++;
    }

    if(IsMouseButtonDown(1) && worldRayCast.distance >= 2000.0f)
    {
        bot.isLocked = false;
    }

    if(bot.isLocked)
    {
        Particle particle;
        particle.teleport(bot.position);
        particle.acceleration += direction;
        sys.particles.push_back(particle);
    }

    else
    {
        sys.particles.clear();
    }

    sys.update();
}

void Restart()
{
    bullet.clear();
    killer.clear();
    bot = (Bot) {};
    bot.health = 1.0f;
    start();
    HideCursor();
}


void RenderVFX(RenderTexture2D * target, RenderTexture2D * sampler)
{

    vfx.at(0).setParameters(0, &bot.isLocked, SHADER_UNIFORM_INT);
    vfx.at(0).setParameters(1, &resolution.x, SHADER_UNIFORM_FLOAT);
    vfx.at(0).setParameters(2, &resolution.y, SHADER_UNIFORM_FLOAT);

    for(auto &v : vfx)
    {
        v.Render(target);
    }
}





void RenderDeath()
{
    DrawRectangle(0, 0, resolution.x, resolution.y, (Color) { 0, 0, 0, 128 });

    float messageWidth = MeasureText("Good Luck Next Time Buddy...", resolution.y / 18);

    DrawText("Good Luck Next Time Buddy...", center.x + ((messageWidth * 0.5f) - messageWidth), center.y * 0.6f, resolution.y / 18, SKYBLUE);

    float messageRespawn = MeasureText("Try Again", resolution.y / 20);



    float2 MousePos = (float2) { (float)GetMouseX(), (float)GetMouseY() };

    float DY = MeasureTextEx(GetFontDefault(), "Try Again", resolution.y / 20, 0).y;

    if(MousePos.x > (center.x - messageRespawn * 0.5f) - 5.0f && MousePos.x < (center.x + messageRespawn * 0.5f) + 10.0f &&
    MousePos.y > (center.y * 0.8f) - 5.0f && MousePos.y < (center.y * 0.8f + DY) + 10.0f)
    {
        DrawText("Try Again", center.x + ((messageRespawn * 0.5f) - messageRespawn), center.y * 0.8f, resolution.y / 20, GREEN);

        if(IsMouseButtonPressed(0))
        {
            Restart();
        }
    }
    else
    {
        DrawText("Try Again", center.x + ((messageRespawn * 0.5f) - messageRespawn), center.y * 0.8f, resolution.y / 20, RED);
    }

    DrawRectangleLines(center.x + ((messageRespawn * 0.5f) - messageRespawn) - 5.0f, (center.y * 0.8f) - 5.0f, messageRespawn + 10.0f, DY + 10.0f, WHITE);
}






void Render()
{
    float2* offset = &cam.position;

    // Drawing Grid
    drawGird2D((float2){50, 50}, 2.0f, *offset);

    
    for(auto &v : bullet)
    {
        // std :: cout << v.position.x << '\n';
        v.Render();
    }

    
    for(auto &k : killer)
    {
        k.Render();
    }


    bot.Render(CursorAngle);

    if(bot.isLocked)
    {
        DrawRectangle(0, 0, resolution.x, resolution.y, (Color) { 0, 0, 0, 128 });
        killer.at(bot.lockedTarget).Render();
    }
    
    // for(auto &s : sys.particles)
    // {
    //     DrawCircle(s.position.x, s.position.y, 5.0f, WHITE);
    // }

    if(bot.health <= 0.0f)
    {
        RenderDeath();
    }
}