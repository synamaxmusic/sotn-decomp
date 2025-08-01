// SPDX-License-Identifier: AGPL-3.0-or-later

typedef enum {
    BLOODY_ZOMBIE_INIT,
    BLOODY_ZOMBIE_WALK,
    BLOODY_ZOMBIE_UNK_2,
    BLOODY_ZOMBIE_CHASE,
    BLOODY_ZOMBIE_ATTACK,
    BLOODY_ZOMBIE_TAKE_HIT = 6,
    BLOODY_ZOMBIE_DYING = 8,
    BLOODY_ZOMBIE_DESTROY
} EntityBloodyZombieSteps;

static s16 sensors_ground[4][2] = {{0, 28}, {0, 4}, {8, -4}, {-16, 0}};
static u16 sensors_move[][2] = {{0, 28}, {8, 0}};
static u8 anim_walk[] = {0x04, 0x02, 0x0D, 0x03, 0x0A, 0x02, 0x0A, 0x01,
                         0x0D, 0x04, 0x0A, 0x01, 0x06, 0x02, 0x00, 0x00};
static u8 anim_attack[] = {
    0x04, 0x05, 0x04, 0x06, 0x03, 0x07, 0x02, 0x08, 0x02, 0x09, 0x04, 0x08,
    0x02, 0x0A, 0x02, 0x0B, 0x02, 0x0C, 0x02, 0x0D, 0x02, 0x0E, 0x02, 0x0F,
    0x0C, 0x10, 0x02, 0x11, 0x02, 0x12, 0x02, 0x13, 0xFF, 0x00};
static u8 anim_hit[] = {
    0x01, 0x16, 0x01, 0x14, 0x01, 0x15, 0x01, 0x14, 0x02, 0x15,
    0x03, 0x14, 0x04, 0x15, 0x08, 0x14, 0x02, 0x16, 0xFF, 0x00};
static u8 anim_die[] = {
    0x01, 0x16, 0x01, 0x14, 0x01, 0x15, 0x01, 0x14, 0x02, 0x15, 0x03, 0x14,
    0x04, 0x15, 0x28, 0x14, 0x06, 0x17, 0x05, 0x18, 0x05, 0x19, 0x05, 0x1A,
    0x05, 0x1B, 0x02, 0x1C, 0x02, 0x1D, 0x02, 0x1E, 0x02, 0x1F, 0x02, 0x1E,
    0x02, 0x1F, 0x02, 0x1E, 0x02, 0x1F, 0x02, 0x1E, 0x02, 0x1F, 0x02, 0x1E,
    0x02, 0x1F, 0x02, 0x1E, 0x02, 0x1F, 0x02, 0x1E, 0x02, 0x1F, 0x02, 0x1E,
    0x02, 0x1F, 0x02, 0x1E, 0x02, 0x1F, 0x02, 0x1E, 0xFF, 0x00};
static u8 anim_chase[] = {0x02, 0x02, 0x06, 0x03, 0x04, 0x02, 0x04, 0x01,
                          0x06, 0x04, 0x04, 0x01, 0x02, 0x02, 0x00, 0x00};
// Use python3 tools/display_texture.py LIVE 0x1A 0x1B7 --whole
// and look at the row of fire-looking images in the row with top at 0x20
// Disable format to keep these as 5-grouped arrays. p2, x, y, w, h.
// clang-format off
static u8 small_blood_anim[] = {
    0x08, 0x40, 0x20, 0x0F, 0x0F,
    0x08, 0x50, 0x20, 0x0F, 0x0F,
    0x08, 0x60, 0x20, 0x0F, 0x0F,
    0x08, 0x70, 0x20, 0x0F, 0x0F,
    0xFF, 0x00};
// Same as above, this looks like a big puffy explosion with red palette
static u8 big_blood_anim[] = {
    0x04, 0x20, 0x00, 0x1F, 0x1F,
    0x04, 0x40, 0x00, 0x1F, 0x1F,
    0x04, 0x60, 0x00, 0x1F, 0x1F,
    0x04, 0x00, 0x20, 0x1F, 0x1F,
    0x04, 0x20, 0x20, 0x1F, 0x0F,
    0x04, 0x20, 0x30, 0x1F, 0x0F,
    0xFF, 0x00};
// clang-format on

static s16 D_801823C4[][2] = {{0x0000, 0x0000}, {0x00FF, 0x0000}};
#if defined(STAGE_IS_NZ0)
static u8 unused[] = {0x06, 0x01, 0x04, 0x01, 0x04, 0x02, 0x06, 0x03,
                      0x05, 0x04, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00};
#endif

void EntityBloodSplatter(Entity* self) {
    Primitive* prim;
    s32 primIndex;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        break;

    case 1:
        primIndex = g_api.AllocPrimitives(PRIM_GT4, 8);
        if (primIndex != -1) {
            self->flags |= FLAG_HAS_PRIMS;
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.bloodSplatter.prim = prim;
        } else {
            DestroyEntity(self);
            return;
        }
        self->step++;
        break;
    case 2:
        prim = self->ext.bloodSplatter.prim;
        prim = FindFirstUnkPrim2(prim, 2);
        if (prim != NULL) {
            self->ext.bloodSplatter.prim2 = prim;
            UnkPolyFunc2(prim);
            prim->tpage = 0x1A;
            prim->clut = 0x1B7;
            prim->u0 = 0x50;
            prim->u1 = 0x50;
            prim->u2 = 0x40;
            prim->u3 = 0x40;
            prim->v0 = 0x30;
            prim->v1 = 0x40;
            prim->v2 = 0x30;
            prim->v3 = 0x40;
            prim->next->b3 = 0x60;
            prim->next->x1 = self->posX.i.hi;
            prim->next->y0 = self->posY.i.hi;
            LOH(prim->next->r2) = 0x10;
            LOH(prim->next->b2) = 0x10;
            LOW(prim->next->r1) = -0x6000;

            if (self->facingLeft) {
                LOW(prim->next->u0) = 0xA000;
                LOH(prim->next->tpage) = 0x200;
                prim->next->x1 += 4;
            } else {
                LOW(prim->next->u0) = -0xA000;
                LOH(prim->next->tpage) = -0x200;
                prim->next->x1 -= 4;
            }
            prim->priority = self->zPriority + 2;
            prim->drawMode = DRAW_COLORS | DRAW_UNK02;
        }

        prim = self->ext.bloodSplatter.prim;
        prim = FindFirstUnkPrim2(prim, 2);
        if (prim != NULL) {
            self->ext.bloodSplatter.prim3 = prim;
            UnkPolyFunc2(prim);
            prim->tpage = 0x1A;
            prim->clut = 0x1B7;
            prim->u0 = 0x20;
            prim->u1 = 0x20;
            prim->u2 = 0;
            prim->u3 = 0;
            prim->v0 = 0x20;
            prim->v1 = 0x40;
            prim->v2 = 0x20;
            prim->v3 = 0x40;

            prim->next->x1 = self->posX.i.hi;
            prim->next->y0 = self->posY.i.hi;
            LOH(prim->next->r2) = 8;
            LOH(prim->next->b2) = 0x10;
            prim->next->b3 = 0x80;
            LOW(prim->next->r1) = -0x8000;
            if (self->facingLeft) {
                LOW(prim->next->u0) = 0xC000;
                LOH(prim->next->tpage) = 0x200;
            } else {
                LOW(prim->next->u0) = -0xC000;
                LOH(prim->next->tpage) = -0x200;
            }
            prim->priority = self->zPriority + 2;
            prim->drawMode = DRAW_COLORS | DRAW_UNK02;
        }
        self->step++;
        break;

    case 3:
        prim = self->ext.bloodSplatter.prim2;
        UnkPrimHelper(prim);

        if (g_Timer % 2) {
            LOH(prim->next->r2)++;
            LOH(prim->next->b2)++;
            prim->clut = 0x1B7;
        } else {
            prim->clut = 0x16D;
        }

        LOW(prim->next->r1) += 0xC00;

        if (self->facingLeft) {
            LOH(prim->next->tpage) += 0x18;
        } else {
            LOH(prim->next->tpage) -= 0x18;
        }

        prim->next->b3 -= 2;
        if (!UpdateAnimation(small_blood_anim, prim)) {
            UnkPolyFunc0(prim);
        }

        prim = self->ext.bloodSplatter.prim3;
        UnkPrimHelper(prim);
        LOH(prim->next->r2) += 2;
        LOH(prim->next->b2) += 2;
        if (prim->p1 > 3) {
            LOH(prim->next->b2) = 16;
        }

        prim->next->b3 -= 3;
        if (prim->next->b3 > 240) {
            prim->next->b3 = 0;
        }

        if (!UpdateAnimation(big_blood_anim, prim)) {
            UnkPolyFunc0(prim);
        }

        if (self->ext.bloodSplatter.unk80++ > 128) {
            DestroyEntity(self);
        }
        break;
    }
}

static void func_801C53AC(Primitive* prim) {
    switch (prim->next->u2) {
    case 0:
        prim->tpage = 0x12;
        prim->clut = 0x16D;
        prim->u0 = 80;
        prim->u1 = 96;
        prim->v1 = 239;
        prim->v0 = 239;
        prim->v3 = 255;
        prim->v2 = 255;
        prim->u2 = prim->u0;
        prim->u3 = prim->u1;
        LOH(prim->next->r2) = 16;
        LOH(prim->next->b2) = 16;
        prim->next->x1 = g_CurrentEntity->posX.i.hi;
        prim->next->y0 = g_CurrentEntity->posY.i.hi;

        if (g_CurrentEntity->facingLeft) {
            prim->next->x1 -= 8;
        } else {
            prim->next->x1 += 8;
        }
        if (prim->next->r3 == 0) {
            LOW(prim->next->u0) = -0x4000;
        } else {
            LOW(prim->next->u0) = 0x4000;
        }
        LOW(prim->next->r1) = -0x20000;
        prim->next->b3 = 0x80;
        prim->priority = g_CurrentEntity->zPriority + 1;
        prim->drawMode = DRAW_UNK02;
        prim->next->u2 = 1;
        break;

    case 1:
        UnkPrimHelper(prim);
        LOW(prim->next->r1) += 0x2000;
        if (LOW(prim->next->r1) > 0x20000) {
            UnkPolyFunc0(prim);
        }
        break;
    }
}

void EntityBloodyZombie(Entity* self) {
    Primitive* prim;
    Entity* newEntity;
    s16 primIndex;
    s16 facing;
    s32 animStatus;

    if (self->hitParams && self->step & 1) {
        PlaySfxPositional(SFX_BLOODY_ZOMBIE_HIT_GRUNT);
        PlaySfxPositional(SFX_BLOODY_ZOMBIE_HIT);
        SetStep(BLOODY_ZOMBIE_TAKE_HIT);
    }

    if (self->flags & FLAG_DEAD && self->step < 8) {
        PlaySfxPositional(SFX_BLOODY_ZOMBIE_DEATH);
        self->hitboxState = 0;
        self->flags &= ~FLAG_UNK_20000000;
        SetStep(BLOODY_ZOMBIE_DYING);
    }

    switch (self->step) {
    case BLOODY_ZOMBIE_INIT:
        InitializeEntity(g_EInitBloodyZombie);
        self->hitboxOffX = 1;
        self->hitboxOffY = 4;
        SetStep(BLOODY_ZOMBIE_UNK_2);
        break;

    case BLOODY_ZOMBIE_UNK_2:
        if (UnkCollisionFunc3(sensors_ground) & 1) {
            SetStep(BLOODY_ZOMBIE_WALK);
        }
        break;

    case BLOODY_ZOMBIE_WALK:
        if (self->step_s == 0) {
            self->ext.bloodyZombie.unk80 = 128;
            self->step_s++;
        }

        AnimateEntity(anim_walk, self);
        UnkCollisionFunc2(sensors_move);

        if (!self->facingLeft) {
            self->velocityX = FIX(-0.375);
        } else {
            self->velocityX = FIX(0.375);
        }

        if (--self->ext.bloodyZombie.unk80 == 0) {
            self->ext.bloodyZombie.unk80 = 128;
            self->facingLeft ^= 1;
        }

        if ((Random() % 64) == 0) { // Drop BloodDrips from the enemy knife
            newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (newEntity != NULL) {
                CreateEntityFromEntity(E_BLOOD_DRIPS, self, newEntity);
                if (self->facingLeft) {
                    newEntity->posX.i.hi += 16;
                } else {
                    newEntity->posX.i.hi -= 16;
                }
                newEntity->posY.i.hi += 12;
            }
        }
        facing = GetSideToPlayer() & 1;
        if (PLAYER.facingLeft == facing && GetDistanceToPlayerX() < 128) {
            self->facingLeft = facing ^ 1;
            SetStep(BLOODY_ZOMBIE_CHASE);
        }
        break;

    case BLOODY_ZOMBIE_CHASE:
        if (!AnimateEntity(anim_chase, self)) {
            self->facingLeft = (GetSideToPlayer() & 1) ^ 1;
        }
        UnkCollisionFunc2(sensors_move);

        if (self->facingLeft) {
            self->velocityX = FIX(0.75);
        } else {
            self->velocityX = FIX(-0.75);
        }

        if ((Random() % 64) == 0) { // Drop BloodDrips from the enemy knife
            newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (newEntity != NULL) {
                CreateEntityFromEntity(E_BLOOD_DRIPS, self, newEntity);
                if (self->facingLeft) {
                    newEntity->posX.i.hi += 18;
                } else {
                    newEntity->posX.i.hi -= 18;
                }
                newEntity->posY.i.hi += 12;
            }
        }

        if (GetDistanceToPlayerX() < 40) {
            SetStep(BLOODY_ZOMBIE_ATTACK);
        }
        break;

    case BLOODY_ZOMBIE_ATTACK:
        animStatus = AnimateEntity(anim_attack, self);
        if (animStatus & 0x80 && self->pose == 10) {
            PlaySfxPositional(SFX_WEAPON_SWISH_B);
        }
        if (animStatus == 0) {
            SetStep(BLOODY_ZOMBIE_WALK);
        }
        break;

    case BLOODY_ZOMBIE_TAKE_HIT:
        if (self->step_s == 0) {
            // Splat blood
            newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (newEntity != NULL) {
                CreateEntityFromEntity(E_BLOOD_SPLATTER, self, newEntity);
                newEntity->facingLeft = GetSideToPlayer() & 1;
            }
            self->step_s++;
        }

        if (!AnimateEntity(anim_hit, self)) {
            SetStep(BLOODY_ZOMBIE_WALK);
            self->step_s++;
        }
        break;

    case BLOODY_ZOMBIE_DYING:
        if (self->step_s == 0) {
            primIndex = g_api.AllocPrimitives(PRIM_GT4, 0x14);
            if (primIndex == -1) {
                DestroyEntity(self);
                return;
            }
            self->primIndex = primIndex;
            prim = &g_PrimBuf[primIndex];
            self->ext.bloodyZombie.prim = prim;
            self->flags |= FLAG_HAS_PRIMS;
            self->step_s++;
        }

        if (self->pose < 13) {
            if ((g_Timer % 8) == 0) {
                PlaySfxPositional(SFX_BLOODY_ZOMBIE_SPLATTER);
                newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
                if (newEntity != NULL) {
                    CreateEntityFromEntity(E_BLOOD_SPLATTER, self, newEntity);
                    newEntity->facingLeft = self->ext.bloodyZombie.unk84;
                    if (self->facingLeft) {
                        newEntity->posX.i.hi -= 4;
                    } else {
                        newEntity->posX.i.hi += 4;
                    }
                    newEntity->posY.i.hi += 4;
                }
                self->ext.bloodyZombie.unk84 ^= 1;
            }
            self->ext.bloodyZombie.unk80 = 0;
        } else {
            if (self->ext.bloodyZombie.unk80 == 0) {
                PlaySfxPositional(SFX_BLOODY_ZOMBIE_SPLATTER);
            }

            self->ext.bloodyZombie.unk80++;
            if ((g_Timer & 3) == 0) {
                prim = FindFirstUnkPrim2(self->ext.bloodyZombie.prim, 2);
                if (prim != NULL) {
                    UnkPolyFunc2(prim);
                    prim->next->r3 = self->ext.bloodyZombie.unk84;
                }
                self->ext.bloodyZombie.unk84 ^= 1;
            }
        }

        if (self->flags & FLAG_HAS_PRIMS) {
            prim = self->ext.bloodyZombie.prim;
            while (prim != NULL) {
                if (prim->p3 & 8) {
                    func_801C53AC(prim);
                }
                prim = prim->next;
            }
        }

        if (!AnimateEntity(anim_die, self)) {
            newEntity = AllocEntity(&g_Entities[224], &g_Entities[256]);
            if (newEntity != NULL) {
                CreateEntityFromEntity(E_EXPLOSION, self, newEntity);
                newEntity->params = 2;
                newEntity->posY.i.hi += 16;
                if (self->facingLeft) {
                    newEntity->posX.i.hi -= 8;
                } else {
                    newEntity->posX.i.hi += 8;
                }
            }
            self->ext.bloodyZombie.unk80 = 64;
            self->animCurFrame = 0;
            PlaySfxPositional(SFX_STUTTER_EXPLODE_LOW);
            self->step++;
        }
        break;

    case BLOODY_ZOMBIE_DESTROY:
        if (self->flags & FLAG_HAS_PRIMS) {
            prim = self->ext.bloodyZombie.prim;
            while (prim != NULL) {
                if (prim->p3 & 8) {
                    func_801C53AC(prim);
                }
                prim = prim->next;
            }
        }

        if (--self->ext.bloodyZombie.unk80 == 0) {
            DestroyEntity(self);
        }
        break;
    }

    if (self->animCurFrame > 10 && self->animCurFrame < 13) {
        self->hitboxWidth = 18;
        self->hitboxHeight = 12;
        *(s16*)&self->hitboxOffX = -12;
        self->hitboxOffY = -12;
    } else {
        self->hitboxWidth = 4;
        self->hitboxHeight = 22;
        self->hitboxOffX = 1;
        self->hitboxOffY = 4;
    }
}

void EntityBloodDrips(Entity* self) { // BloodDrips
    Primitive* prim;
    u32 primIndex;
    u32 i;

    switch (self->step) {
    case 0:
        InitializeEntity(g_EInitParticle);
        primIndex = g_api.AllocPrimitives(PRIM_LINE_G2, 1);
        if (primIndex != -1) {
            self->primIndex = primIndex;
            self->flags |= FLAG_HAS_PRIMS;
            self->hitboxState = 0;
            prim = &g_PrimBuf[primIndex];
            self->ext.prim = prim;
            // i iterator here is otherwise unused but needed for PSP match
            for (i = 0; prim != NULL; i++, prim = prim->next) {
                prim->x0 = prim->x1 = self->posX.i.hi;
                prim->y0 = prim->y1 = self->posY.i.hi;
                prim->r0 = 255;
                prim->r1 = 32;
                prim->g0 = 0;
                prim->g1 = 0;
                prim->b0 = 48;
                prim->b1 = 16;
                prim->priority = self->zPriority + 1;
                prim->drawMode |= DRAW_TPAGE2 | DRAW_TPAGE | DRAW_COLORS |
                                  DRAW_UNK02 | DRAW_TRANSP;
            }
        } else {
            DestroyEntity(self);
        }
        break;

    case 1:
        prim = self->ext.prim;
        if (CheckColliderOffsets(&D_801823C4, 0) != 0) {
            prim->y1 += 2;
            if (!self->step_s) {
                self->step_s = 1;
            }
        } else {
            self->velocityY += FIX(0.09375);
            self->posY.val += self->velocityY;
            if ((prim->y0 - prim->y1) > 8) {
                prim->y1 = prim->y0 - 8;
            }
        }
        prim->x0 = self->posX.i.hi;
        prim->x1 = self->posX.i.hi;
        prim->y0 = self->posY.i.hi;
        if (prim->y0 < prim->y1) {
            g_api.FreePrimitives(self->primIndex);
            DestroyEntity(self);
        }
        break;
    }
}
